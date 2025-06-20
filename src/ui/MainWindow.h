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
    void onPlayButtonClicked();
    void onPauseButtonClicked();
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

private:
    Core _core;
    PlaylistView* _playlistView;
    QComboBox* _playlistComboBox; // 歌单下拉框

    // 播放控制按钮
    QPushButton *_playButton;
    QPushButton *_pauseButton;
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

    // 歌曲信息
    QLabel *_currentSongLabel;
    QLabel *_currentTimeLabel;
    QLabel *_totalTimeLabel;
    QSlider *_positionSlider;

    void _updateCurrentSongLabel();
    void _updateTimeLabels();
    void _updatePositionSlider();

    void _refreshPlaylistComboBox(); // 刷新歌单下拉框
    int _currentPlaylistIndex; // 当前选择的歌单索引
};
