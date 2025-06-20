#include "MainWindow.h"
#include <iostream>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug> // 添加调试信息
#include <QPointer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _volumeButtonsVisible(false), _currentPlaylistIndex(0) {

    _core.loadPlaylistsFromFile("playlists.txt"); // 加载播放列表

    // 创建 UI 元素
    _playButton = new QPushButton("Play", this);
    _pauseButton = new QPushButton("Pause", this);
    _stopButton = new QPushButton("Stop", this);
    _previousButton = new QPushButton("Previous", this);
    _nextButton = new QPushButton("Next", this);
    _addButton = new QPushButton("Add Song", this);

    _volumeButton = new QPushButton("Volume", this);
    _increaseVolumeButton = new QPushButton("+", this);
    _decreaseVolumeButton = new QPushButton("-", this);
    _volumeSlider = new QSlider(Qt::Horizontal, this);
    _volumeLabel = new QLabel("Volume: 100%", this);

    _currentSongLabel = new QLabel("No song selected", this);
    _currentTimeLabel = new QLabel("00:00", this);
    _totalTimeLabel = new QLabel("00:00", this);
    _positionSlider = new QSlider(Qt::Horizontal, this);

    _playlistView = new PlaylistView(this);
    _playlistView->setContextMenuPolicy(Qt::CustomContextMenu); // 允许自定义右键菜单

    _playlistComboBox = new QComboBox(this);  // 歌单下拉框
    _refreshPlaylistComboBox(); // 刷新歌单下拉框

    _playlistView->refreshPlaylist(_core.getPlaylist(_currentPlaylistIndex)); // 刷新播放列表

    _volumeSlider->setRange(0, 100);
    _volumeSlider->setValue(100);
    _positionSlider->setRange(0, 100);
    _positionSlider->setValue(0);

    // 设置音量按钮初始状态
    _increaseVolumeButton->hide();
    _decreaseVolumeButton->hide();

    // 布局
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(_previousButton);
    controlLayout->addWidget(_playButton);
    controlLayout->addWidget(_pauseButton);
    controlLayout->addWidget(_stopButton);
    controlLayout->addWidget(_nextButton);
    controlLayout->addWidget(_addButton);

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(_currentTimeLabel);
    timeLayout->addWidget(_positionSlider);
    timeLayout->addWidget(_totalTimeLabel);

    QHBoxLayout *volumeControlLayout = new QHBoxLayout;
    volumeControlLayout->addWidget(_volumeButton);
    volumeControlLayout->addWidget(_decreaseVolumeButton);
    volumeControlLayout->addWidget(_increaseVolumeButton);
    volumeControlLayout->addWidget(_volumeLabel);
    volumeControlLayout->addWidget(_volumeSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_playlistComboBox); // 添加歌单下拉框
    mainLayout->addWidget(_currentSongLabel);
    mainLayout->addLayout(timeLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(volumeControlLayout);
    mainLayout->addWidget(_playlistView);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(_playButton, &QPushButton::clicked, this, &MainWindow::onPlayButtonClicked);
    connect(_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseButtonClicked);
    connect(_stopButton, &QPushButton::clicked, this, &MainWindow::onStopButtonClicked);
    connect(_previousButton, &QPushButton::clicked, this, &MainWindow::onPreviousButtonClicked);
    connect(_nextButton, &QPushButton::clicked, this, &MainWindow::onNextButtonClicked);
    connect(_addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(_volumeButton, &QPushButton::clicked, this, &MainWindow::onVolumeButtonClicked);
    connect(_increaseVolumeButton, &QPushButton::clicked, this, &MainWindow::onIncreaseVolumeButtonClicked);
    connect(_decreaseVolumeButton, &QPushButton::clicked, this, &MainWindow::onDecreaseVolumeButtonClicked);
    connect(_volumeSlider, &QSlider::valueChanged, this, &MainWindow::onVolumeSliderValueChanged);
    connect(_playlistView, &PlaylistView::itemClicked, this, &MainWindow::onPlaylistItemSelected);
    connect(_positionSlider, &QSlider::valueChanged, this, &MainWindow::onPositionSliderValueChanged); // 连接松开信号

    // 右键菜单
    connect(_playlistView, &PlaylistView::customContextMenuRequested, this, &MainWindow::onFavoriteSongRequested);

    // 歌单下拉框
    connect(_playlistComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onPlaylistComboBoxChanged);

    // 定时器更新时间标签
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::_updateTimeLabels);
    timer->start(1000); // 每秒更新一次

    // 初始化
    setWindowTitle("Music Player");
    resize(800, 600);
}

MainWindow::~MainWindow() {
    delete _playButton;
    delete _pauseButton;
    delete _stopButton;
    delete _previousButton;
    delete _nextButton;
    delete _addButton;
    delete _volumeButton;
    delete _increaseVolumeButton;
    delete _decreaseVolumeButton;
    delete _volumeSlider;
    delete _volumeLabel;
    delete _currentSongLabel;
    delete _currentTimeLabel;
    delete _totalTimeLabel;
    delete _positionSlider;
    delete _playlistView;
    delete _playlistComboBox;
}

void MainWindow::onPlayButtonClicked() {
    _core.play();
}

void MainWindow::onPauseButtonClicked() {
    _core.pause();
}

void MainWindow::onStopButtonClicked() {
    _core.stop();
}

void MainWindow::onPreviousButtonClicked() {
    // 实现上一首逻辑
}

void MainWindow::onNextButtonClicked() {
    // 实现下一首逻辑
}

void MainWindow::onAddButtonClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select a Song", "", "Audio Files (*.mp3 *.wav *.flac)");
    if (!filePath.isEmpty()) {
        _core.addSong(_currentPlaylistIndex, filePath.toStdString()); // 添加到当前歌单
        _core.savePlaylistsToFile("playlists.txt");
        _playlistView->refreshPlaylist(_core.getPlaylist(_currentPlaylistIndex));
    }
}

void MainWindow::onVolumeButtonClicked() {
    _volumeButtonsVisible = !_volumeButtonsVisible;
    _increaseVolumeButton->setVisible(_volumeButtonsVisible);
    _decreaseVolumeButton->setVisible(_volumeButtonsVisible);
}

void MainWindow::onIncreaseVolumeButtonClicked() {
    int volume = _volumeSlider->value();
    volume = std::min(volume + 10, 100); // 每次增加 10，最大 100
    _volumeSlider->setValue(volume);
}

void MainWindow::onDecreaseVolumeButtonClicked() {
    int volume = _volumeSlider->value();
    volume = std::max(volume - 10, 0); // 每次减少 10，最小 0
    _volumeSlider->setValue(volume);
}

void MainWindow::onVolumeSliderValueChanged(int value) {
    float volume = static_cast<float>(value) / 100.0f;
    _core.setVolume(volume);
    _volumeLabel->setText("Volume: " + QString::number(value) + "%");
}

void MainWindow::onPlaylistItemSelected(QListWidgetItem *item) {
    int index = _playlistView->row(item);
    if (index >= 0) {
        _core.playSong(_currentPlaylistIndex, index); // 播放当前歌单的歌曲
        _updateCurrentSongLabel();
    }
}

void MainWindow::onPositionSliderValueChanged() {
    int value = _positionSlider->value(); // 获取滑块位置
    // 实现跳转播放进度的逻辑，需要调用 Core 的方法
    // 例如：_core.seek(value);
}

void MainWindow::onFavoriteSongRequested(QPoint pos) {
    QListWidgetItem *item = _playlistView->itemAt(pos);
    if (!item) return;

    // 使用栈上菜单，自动管理生命周期
    QMenu menu(this);

    // 存储索引以避免lambda捕获悬空指针
    const int songIndex = _playlistView->row(item);

    // 添加菜单项
    QAction *newAction = menu.addAction("New Playlist");
    QAction *addExisting = menu.addAction("Add to Existing Playlist");
    QAction *addFavorite = menu.addAction("Add to Favorite Playlist");

    // 显示菜单并等待用户选择
    QAction *selectedAction = menu.exec(_playlistView->viewport()->mapToGlobal(pos));

    // 菜单会在离开作用域时自动删除

    // 处理选择的动作
    if (selectedAction == newAction) {
        bool ok;
        QString playlistName = QInputDialog::getText(this, "New Playlist", "Playlist name:",
                                                   QLineEdit::Normal, "", &ok);
        if (ok && !playlistName.isEmpty()) {
            _core.createPlaylist(playlistName.toStdString());
            _refreshPlaylistComboBox();
            _core.savePlaylistsToFile("playlists.txt");
            qDebug() << "Creating playlist:" << playlistName << "for song" << songIndex;
        }
    }
    else if (selectedAction == addExisting) {
        QMessageBox::information(this, "Info", "Adding to existing playlist");
    }
    else if (selectedAction == addFavorite) {
        QMessageBox::information(this, "Info", "Adding to favorites");
    }
}

void MainWindow::_updateCurrentSongLabel() {
    int playlistSize = _core.getPlaylistSize(_currentPlaylistIndex);
    if (playlistSize > 0 && _core.isPlaying()) {
        // 假设播放器按照playlist index来播放
        PlaylistEntry song = _core.getSong(_currentPlaylistIndex, 0);
        _currentSongLabel->setText(QString::fromStdString(song._title + " - " + song._artist));
    } else {
        _currentSongLabel->setText("No song selected");
    }
}

void MainWindow::_updateTimeLabels() {
    // 获取当前播放时间点和总时间
    // 需要调用 Core 的方法，假设有 getCurrentPosition 和 getTotalDuration 方法
    // 示例：
    // int currentPosition = _core.getCurrentPosition(); // 单位：秒
    // int totalDuration = _core.getTotalDuration(); // 单位：秒

    // QTime currentTime(currentPosition / 3600, (currentPosition % 3600) / 60, currentPosition % 60);
    // QTime totalTime(totalDuration / 3600, (totalDuration % 3600) / 60, totalDuration % 60);

    // _currentTimeLabel->setText(currentTime.toString("mm:ss"));
    // _totalTimeLabel->setText(totalTime.toString("mm:ss"));

    // _updatePositionSlider(); // 同时更新进度条
}

void MainWindow::_updatePositionSlider() {
    // 更新进度条
    // 需要调用 Core 的方法，获取当前播放进度百分比
    // 示例：
    // float positionPercentage = _core.getPositionPercentage(); // 0.0 - 1.0
    // int sliderValue = static_cast<int>(positionPercentage * 100.0f);
    // _positionSlider->setValue(sliderValue);
}

void MainWindow::_refreshPlaylistComboBox() {
    _playlistComboBox->clear();
    int playlistCount = _core.getPlaylistCount();
    for (int i = 0; i < playlistCount; ++i) {
        _playlistComboBox->addItem(QString::fromStdString(_core.getPlaylist(i).getName()));
    }
}

void MainWindow::onPlaylistComboBoxChanged(int index) {
    if (index >= 0 && index < _core.getPlaylistCount()) {
        _currentPlaylistIndex = index;
        _playlistView->refreshPlaylist(_core.getPlaylist(_currentPlaylistIndex));
    }
}
