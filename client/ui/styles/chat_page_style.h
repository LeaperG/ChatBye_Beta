#pragma once

#include "client/pages/chat_page.h"
#include <QSettings>

class ChatPageStyle
{
public:
    explicit ChatPageStyle();
    void setStyle(ChatPage* page);

private:
    QSettings settings;
};

