#pragma once

#include <QListWidget>
#include "Core.h"
#include <QMouseEvent> // 添加鼠标事件支持

class PlaylistView : public QListWidget {
    Q_OBJECT

public:
    PlaylistView(QWidget *parent = nullptr);
    ~PlaylistView() override;

    void refreshPlaylist(const Playlist& playlist);
    void contextMenuEvent(QContextMenuEvent *event) override; // 重写右键菜单事件处理函数
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void itemClicked(QListWidgetItem *item);
    void customContextMenuRequested(const QPoint &pos); // 添加右键菜单信号

protected:
    

};