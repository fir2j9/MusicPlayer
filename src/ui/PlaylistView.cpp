#include "PlaylistView.h"
#include <QListWidgetItem>
#include "PlaylistEntry.h"
#include <QContextMenuEvent>
#include <QDebug> // 添加调试信息

PlaylistView::PlaylistView(QWidget *parent) : QListWidget(parent) {
    connect(this, &QListWidget::itemClicked, this, &PlaylistView::itemClicked);
    setContextMenuPolicy(Qt::CustomContextMenu); // 设置右键菜单策略

    // 允许选择和交互
    setSelectionMode(QAbstractItemView::SingleSelection);
    setFocusPolicy(Qt::StrongFocus);
    setAcceptDrops(true);
}

PlaylistView::~PlaylistView() {}

void PlaylistView::refreshPlaylist(const Playlist& playlist) {
    clear();
    int playlistSize = playlist.getSize();
    for (int i = 0; i < playlistSize; ++i) {
        PlaylistEntry song = playlist.getSong(i);
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(song.getDisplayName()));
        addItem(item);
    }
}

// 重要：处理鼠标按下事件
void PlaylistView::mousePressEvent(QMouseEvent *event) {
    // qDebug() << "Mouse Press Event:" << event->button();
    
    if (event->button() == Qt::RightButton) {
        // qDebug() << "Right button pressed at:" << event->pos();
        QListWidgetItem* item = itemAt(event->pos());
        if (item) {
            setCurrentItem(item);
            emit customContextMenuRequested(event->pos());
            event->accept(); // 接受事件
            return; // 重要：处理完毕直接返回
        }
    }
    
    // 对于其他鼠标按钮，调用基类实现
    QListWidget::mousePressEvent(event);
}

void PlaylistView::contextMenuEvent(QContextMenuEvent *event) {

    // qDebug() << "Context menu requested at:" << event->pos();
    // qDebug() << "Selected item:" << (currentItem() ? currentItem()->text() : "None");

    if (QListWidgetItem *item = itemAt(event->pos())) {
        setCurrentItem(item); // 确保选择当前项
        emit customContextMenuRequested(event->pos());
        event->accept();      // 标记事件已处理
    } else {
        event->ignore();      // 允许传递到父组件
    }
}
