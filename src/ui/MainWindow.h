#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QListWidget>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QComboBox> // 添加 QComboBox
#include "Core.h"
#include "PlaylistView.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onModeButtonClicked();
    void onPauseOrPlayButtonClicked();
    void onStopButtonClicked();
    void onPreviousButtonClicked();
    void onNextButtonClicked();
    void onAddButtonClicked();
    void onVolumeButtonClicked();
    void onIncreaseVolumeButtonClicked();
    void onDecreaseVolumeButtonClicked();
    void onVolumeSliderValueChanged(int value);
    void onPlaylistItemSelected(QListWidgetItem *item);
    void onPositionSliderValueChanged(); // 进度条
    void onPlaylistComboBoxChanged(int index); // 歌单切换

    void onFavoriteSongRequested(QPoint pos); // 收藏歌曲
    void onSelectPlayModeRequested(QPoint pos); // 选择播放模式

private:
    Core _core;
    PlaylistView* _playlistView;
    QComboBox* _playlistComboBox; // 歌单下拉框

    QPushButton* _modeButton; // 模式按钮

    // 播放控制按钮
    QPushButton *_pauseOrPlayButton; // 播放/暂停按钮
    QPushButton *_stopButton;
    QPushButton *_previousButton;
    QPushButton *_nextButton;

    // 添加歌曲按钮
    QPushButton *_addButton;

    // 音量控制
    QPushButton *_volumeButton; // 音量按钮
    QPushButton *_increaseVolumeButton; // 增加音量按钮
    QPushButton *_decreaseVolumeButton; // 降低音量按钮
    QSlider *_volumeSlider;
    QLabel *_volumeLabel;
    bool _volumeButtonsVisible; // 音量按钮是否可见

    const std::string PLAY_LIST_FILE = "playlists.txt"; // 歌单文件名

    // 歌曲信息
    QLabel *_currentSongLabel;
    QLabel *_currentTimeLabel;
    QLabel *_totalTimeLabel;
    QSlider *_positionSlider;

    void _refreshUI(); // 刷新界面

    void _updateCurrentSongLabel();
    void _updateTimeLabels();
    void _updatePositionSlider();
    void _updatePlayStateLabel();
    void _updatePlayModeButton();

    void _updatePlaylistComboBox(); // 刷新歌单下拉框
};
