#include "FfmpegView.h"

FfmpegView::FfmpegView(QWidget* parent) : QWidget(parent),
ui(new Ui::FfmpegViewClass),
decoder(new VideoDecoder),
menuTimer(new QTimer),
progressTimer(new QTimer),
menuIsVisible(true),
isKeepAspectRatio(false),
image(QImage(":/image/MUSIC.jpg")),
autoPlay(true),
loopPlay(false),
closeNotExit(false),
playState(VideoDecoder::STOP),
seekInterval(15)
{
    ui->setupUi(this);
    qRegisterMetaType<VideoDecoder::PlayState>("Decoder::PlayState");
    menuTimer->setInterval(8000);
    menuTimer->start(5000);
    progressTimer->setInterval(500);
    //init UI
    this->setMouseTracking(true);
    ui->titleLable->setAlignment(Qt::AlignCenter);
    ui->labelTime->setStyleSheet("background: #5FFFFFFF;");
    ui->labelTime->setText(QString("00.00.00 / 00:00:00"));
    ui->btnNext->setIcon(QIcon(":/image/next.ico"));
    ui->btnNext->setIconSize(QSize(48, 48));
    ui->btnNext->setStyleSheet("background: transparent;border:none;");
    ui->btnPreview->setIcon(QIcon(":/image/forward.ico"));
    ui->btnPreview->setIconSize(QSize(48, 48));
    ui->btnPreview->setStyleSheet("background: transparent;border:none;");
    ui->btnStop->setIcon(QIcon(":/image/stop.ico"));
    ui->btnStop->setIconSize(QSize(48, 48));
    ui->btnStop->setStyleSheet("background: transparent;border:none;");
    ui->btnPause->setIcon(QIcon(":/image/play.ico"));
    ui->btnPause->setIconSize(QSize(48, 48));
    ui->btnPause->setStyleSheet("background: transparent;border:none;");
    setHide(ui->btnOpenLocal);
    setHide(ui->btnOpenUrl);
    setHide(ui->btnStop);
    setHide(ui->btnPause);
    setHide(ui->btnNext);
    setHide(ui->btnPreview);
    setHide(ui->lineEdit);
    setHide(ui->videoProgressSlider);
    setHide(ui->labelTime);
    ui->videoProgressSlider->installEventFilter(this);
    //init Tray
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip(tr("QtPlayer"));
    trayIcon->setIcon(QIcon(":/image/player.ico"));
    trayIcon->show();
    QAction* minimizeAction = new QAction(tr("最小化 (&I)"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    QAction* restoreAction = new QAction(tr("还原 (&R)"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    QAction* quitAction = new QAction(tr("退出 (&Q)"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    QMenu* trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    //init Slot
    connect(ui->btnOpenLocal, SIGNAL(clicked(bool)), this, SLOT(buttonClickSlot()));
    connect(ui->btnOpenUrl, SIGNAL(clicked(bool)), this, SLOT(buttonClickSlot()));
    connect(ui->btnStop, SIGNAL(clicked(bool)), this, SLOT(buttonClickSlot()));
    connect(ui->btnPause, SIGNAL(clicked(bool)), this, SLOT(buttonClickSlot()));
    connect(ui->btnNext, SIGNAL(clicked(bool)), this, SLOT(buttonClickSlot()));
    connect(ui->btnPreview, SIGNAL(clicked(bool)), this, SLOT(buttonClickSlot()));
    connect(ui->lineEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(editText()));
    connect(menuTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    connect(progressTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    connect(ui->videoProgressSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekProgress(int)));
    connect(this, SIGNAL(selectedVideoFile(QString, QString)), decoder, SLOT(decoderFile(QString, QString)));
    connect(this, SIGNAL(stopVideo()), decoder, SLOT(stopVideo()));
    connect(this, SIGNAL(pauseVideo()), decoder, SLOT(pauseVideo()));
    connect(decoder, SIGNAL(playStateChanged(Decoder::PlayState)), this, SLOT(playStateChanged(Decoder::PlayState)));
    connect(decoder, SIGNAL(gotVideoTime(qint64)), this, SLOT(videoTime(qint64)));
    connect(decoder, SIGNAL(gotVideo(QImage)), this, SLOT(showVideo(QImage)));
    //init FFmpeg
    avfilter_register_all();
    av_register_all();
    if (avformat_network_init()) {
        qDebug() << "avformat network init failed";
    }
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        qDebug() << "SDL init failed";
    }
}
FfmpegView::~FfmpegView()
{
    delete ui;
}
void FfmpegView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int width = this->width();
    int height = this->height();
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, width, height);
    if (isKeepAspectRatio) {
        QImage img = image.scaled(QSize(width, height), Qt::KeepAspectRatio);
        int x = (this->width() - img.width()) / 2;
        int y = (this->height() - img.height()) / 2;
        painter.drawImage(QPoint(x, y), img);
    }
    else {
        QImage img = image.scaled(QSize(width, height));
        painter.drawImage(QPoint(0, 0), img);
    }
}
void FfmpegView::closeEvent(QCloseEvent* event)
{
    if (closeNotExit) {
        event->ignore();
        this->hide();
    }
}
void FfmpegView::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
        if (this->windowState() == Qt::WindowMinimized) {
            event->ignore();
            this->hide();
        }
    }
}
bool FfmpegView::eventFilter(QObject* object, QEvent* event)
{
    if (object == ui->videoProgressSlider) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                int duration = ui->videoProgressSlider->maximum() - ui->videoProgressSlider->minimum();
                int pos = ui->videoProgressSlider->minimum() + duration * (static_cast<double>(mouseEvent->x()) / ui->videoProgressSlider->width());
                if (pos != ui->videoProgressSlider->sliderPosition()) {
                    ui->videoProgressSlider->setValue(pos);
                    decoder->seekProgress(static_cast<qint64>(pos) * 1000000);
                }
            }
        }
    }
    return QObject::eventFilter(object, event);
}
void FfmpegView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->buttons() == Qt::LeftButton) {
        if (isFullScreen()) {
            showNormal();
        }
        else {
            showFullScreen();
        }
    }
}
void FfmpegView::showPlayMenu()
{
    QMenu* menu = new QMenu;
    QAction* fullSrcAction = new QAction("全屏", this);
    fullSrcAction->setCheckable(true);
    if (isFullScreen()) {
        fullSrcAction->setChecked(true);
    }
    QAction* keepRatioAction = new QAction("视频长宽比", this);
    keepRatioAction->setCheckable(true);
    if (isKeepAspectRatio) {
        keepRatioAction->setChecked(true);
    }
    QAction* autoPlayAction = new QAction("连续播放", this);
    autoPlayAction->setCheckable(true);
    if (autoPlay) {
        autoPlayAction->setChecked(true);
    }
    QAction* loopPlayAction = new QAction("循环播放", this);
    loopPlayAction->setCheckable(true);
    if (loopPlay) {
        loopPlayAction->setChecked(true);
    }
    QAction* captureAction = new QAction("截图", this);
    connect(fullSrcAction, SIGNAL(triggered(bool)), this, SLOT(setFullScreen()));
    connect(keepRatioAction, SIGNAL(triggered(bool)), this, SLOT(setKeepRatio()));
    connect(autoPlayAction, SIGNAL(triggered(bool)), this, SLOT(setAutoPlay()));
    connect(loopPlayAction, SIGNAL(triggered(bool)), this, SLOT(setLoopPlay()));
    connect(captureAction, SIGNAL(triggered(bool)), this, SLOT(saveCurrentFrame()));
    menu->addAction(fullSrcAction);
    menu->addAction(keepRatioAction);
    menu->addAction(autoPlayAction);
    menu->addAction(loopPlayAction);
    menu->addAction(captureAction);
    menu->exec(QCursor::pos());
    disconnect(fullSrcAction, SIGNAL(triggered(bool)), this, SLOT(setFullScreen()));
    disconnect(keepRatioAction, SIGNAL(triggered(bool)), this, SLOT(setKeepRatio()));
    disconnect(autoPlayAction, SIGNAL(triggered(bool)), this, SLOT(setAutoPlay()));
    disconnect(loopPlayAction, SIGNAL(triggered(bool)), this, SLOT(setLoopPlay()));
    disconnect(captureAction, SIGNAL(triggered(bool)), this, SLOT(saveCurrentFrame()));
    delete fullSrcAction;
    delete keepRatioAction;
    delete autoPlayAction;
    delete loopPlayAction;
    delete captureAction;
    delete menu;
}
void FfmpegView::setHide(QWidget* widget)
{
    hideVector.push_back(widget);
}
void FfmpegView::showControl(bool show)
{
    if (show) {
        for (QWidget* widget : hideVector) {
            widget->show();
        }
    }
    else {
        for (QWidget* widget : hideVector) {
            widget->hide();
        }
    }
}
inline QString FfmpegView::getFilenameFromPath(QString path)
{
    return path.right(path.size() - path.lastIndexOf("/") - 1);
}
QString FfmpegView::fileType(QString file)
{
    QString type;
    QString suffix = file.right(file.size() - file.lastIndexOf(".") - 1);
    if (suffix == "mp3" || suffix == "ape" || suffix == "flac" || suffix == "wav") {
        type = "music";
    }
    else {
        type = "video";
    }
    return type;
}
void FfmpegView::addPathVideoToList(QString path)
{
    QDir dir(path);
    QRegularExpression rx(".*\\.(264|rmvb|flv|mp4|mov|avi|mkv|ts|wav|flac|ape|mp3)$");
    QFileInfoList list = dir.entryInfoList(QDir::Files);
    for (int i = 0; i < list.count(); i++) {
        QFileInfo fileInfo = list.at(i);    
        QRegularExpressionMatch match = rx.match(fileInfo.fileName());
        if (match.hasMatch()) {
            QString filename = getFilenameFromPath(fileInfo.fileName());
            if (!playList.contains(filename)) {
                playList.push_back(fileInfo.absoluteFilePath());
            }
        }
    }
}
void FfmpegView::playVideo(QString file)
{
    stopVideo();
    currentPlay = file;
    currentPlayType = fileType(file);
    if (currentPlayType == "video") {
        menuTimer->start();
        ui->titleLable->setText("");
    }
    else {
        menuTimer->stop();
        if (!menuIsVisible) {
            showControl(true);
            menuIsVisible = true;
        }
        ui->titleLable->setStyleSheet("color:rgb(25, 125, 203);font-size:24px;background: transparent;");
        ui->titleLable->setText(QString("当前播放：%1").arg(getFilenameFromPath(file)));
    }
    selectedVideoFile(file, currentPlayType);
}
void FfmpegView::playNext()
{
    int playIndex = 0;
    int videoNum = playList.size();
    if (videoNum <= 0) {
        return;
    }
    int currentIndex = playList.indexOf(currentPlay);
    if (currentIndex != videoNum - 1) {
        playIndex = currentIndex + 1;
    }
    QString nextVideo = playList.at(playIndex);
    QFile file(nextVideo);
    if (!file.exists()) {
        playList.removeAt(playIndex);
        return;
    }
    playVideo(nextVideo);
}
void FfmpegView::playPreview()
{
    int playIndex = 0;
    int videoNum = playList.size();
    int currentIndex = playList.indexOf(currentPlay);
    if (videoNum <= 0) {
        return;
    }
    if (currentIndex > 0) {
        playIndex = currentIndex - 1;
    }
    else {
        playIndex = videoNum - 1;
    }
    QString preVideo = playList.at(playIndex);
    QFile file(preVideo);
    if (!file.exists()) {
        playList.removeAt(playIndex);
        return;
    }
    playVideo(preVideo);
}
void FfmpegView::buttonClickSlot()
{
    QString filePath;
    if (QObject::sender() == ui->btnOpenLocal) {
        filePath = QFileDialog::getOpenFileName(
            this, "选择播放文件", "/",
            "(*.264 *.mp4 *.rmvb *.avi *.mov *.flv *.mkv *.ts *.mp3 *.flac *.ape *.wav)");
        if (!filePath.isNull() && !filePath.isEmpty()) {
            playVideo(filePath);
            QString path = filePath.left(filePath.lastIndexOf("/") + 1);
            addPathVideoToList(path);
        }
    }
    else if (QObject::sender() == ui->btnOpenUrl) {
        filePath = ui->lineEdit->text();
        if (!filePath.isNull() && !filePath.isEmpty()) {
            QString type = "video";
            selectedVideoFile(filePath, type);
        }
    }
    else if (QObject::sender() == ui->btnStop) {
        stopVideo();
    }
    else if (QObject::sender() == ui->btnPause) {
        pauseVideo();
    }
    else if (QObject::sender() == ui->btnPreview) {
        playPreview();
    }
    else if (QObject::sender() == ui->btnNext) {
        playNext();
    }
}
void FfmpegView::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        this->raise();
        this->activateWindow();
        break;
    case QSystemTrayIcon::Trigger:
    default:
        break;
    }
}
void FfmpegView::setFullScreen()
{
    if (isFullScreen()) {
        showNormal();
    }
    else {
        showFullScreen();
    }
}
void FfmpegView::setKeepRatio()
{
    isKeepAspectRatio = !isKeepAspectRatio;
}
void FfmpegView::setAutoPlay()
{
    autoPlay = !autoPlay;
    loopPlay = false;
}
void FfmpegView::setLoopPlay()
{
    loopPlay = !loopPlay;
    autoPlay = false;
}
void FfmpegView::saveCurrentFrame()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存截图", "/", "(*.jpg)");
    image.save(filename);
}
void FfmpegView::timerSlot()
{
    if (QObject::sender() == menuTimer) {
        if (menuIsVisible && playState == VideoDecoder::PLAYING) {
            if (isFullScreen()) {
                QApplication::setOverrideCursor(Qt::BlankCursor);
            }
            showControl(false);
            menuIsVisible = false;
        }
    }
    else if (QObject::sender() == progressTimer) {
        qint64 currentTime = static_cast<qint64>(decoder->getCurrentTime());
        ui->videoProgressSlider->setValue(currentTime);
        int hourCurrent = currentTime / 60 / 60;
        int minCurrent = (currentTime / 60) % 60;
        int secCurrent = currentTime % 60;
        int hourTotal = timeTotal / 60 / 60;
        int minTotal = (timeTotal / 60) % 60;
        int secTotal = timeTotal % 60;
        ui->labelTime->setText(QString("%1.%2.%3 / %4:%5:%6")
            .arg(hourCurrent, 2, 10, QLatin1Char('0'))
            .arg(minCurrent, 2, 10, QLatin1Char('0'))
            .arg(secCurrent, 2, 10, QLatin1Char('0'))
            .arg(hourTotal, 2, 10, QLatin1Char('0'))
            .arg(minTotal, 2, 10, QLatin1Char('0'))
            .arg(secTotal, 2, 10, QLatin1Char('0')));
    }
}

void FfmpegView::seekProgress(int value)
{
    decoder->seekProgress(static_cast<qint64>(value) * 1000000);
}
void FfmpegView::videoTime(qint64 time)
{
    timeTotal = time / 1000000;
    ui->videoProgressSlider->setRange(0, timeTotal);
    int hour = timeTotal / 60 / 60;
    int min = (timeTotal / 60) % 60;
    int sec = timeTotal % 60;
    ui->labelTime->setText(QString("00.00.00 / %1:%2:%3").arg(hour, 2, 10, QLatin1Char('0'))
        .arg(min, 2, 10, QLatin1Char('0'))
        .arg(sec, 2, 10, QLatin1Char('0')));
}
void FfmpegView::showVideo(QImage image)
{
    this->image = image;
    update();
}
void FfmpegView::playStateChanged(VideoDecoder::PlayState state)
{
    switch (state) {
    case VideoDecoder::PLAYING:
        ui->btnPause->setIcon(QIcon(":/image/pause.ico"));
        playState = VideoDecoder::PLAYING;
        progressTimer->start();
        break;
    case VideoDecoder::STOP:
        image = QImage(":/image/MUSIC.jpg");
        ui->btnPause->setIcon(QIcon(":/image/play.ico"));
        playState = VideoDecoder::STOP;
        progressTimer->stop();
        ui->labelTime->setText(QString("00.00.00 / 00:00:00"));
        ui->videoProgressSlider->setValue(0);
        timeTotal = 0;
        update();
        break;
    case VideoDecoder::PAUSE:
        ui->btnPause->setIcon(QIcon(":/image/play.ico"));
        playState = VideoDecoder::PAUSE;
        break;
    case VideoDecoder::FINISH:
        if (autoPlay) {
            playNext();
        }
        else if (loopPlay) {
            selectedVideoFile(currentPlay, currentPlayType);
        }
        else {
            image = QImage(":/image/MUSIC.jpg");
            playState = VideoDecoder::STOP;
            progressTimer->stop();
            ui->labelTime->setText(QString("00.00.00 / 00:00:00"));
            ui->videoProgressSlider->setValue(0);
            timeTotal = 0;
        }
        break;
    }
}
void FfmpegView::selectedVideoFile(QString file, QString type) {
    emit selectedVideoFile(currentPlay, currentPlayType);
}
void FfmpegView::stopVideo() {
    emit stopVideo();
}
void FfmpegView::pauseVideo() {
    emit pauseVideo();
}