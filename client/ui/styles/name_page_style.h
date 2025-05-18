#pragma once

#include "client/pages/name_page.h"
#include <QSettings>

class NamePageStyle
{
public:
    explicit NamePageStyle();
    void setStyle(NamePage* page);

private:
    QSettings settings;
};

