#pragma once

#include "client/pages/port_page.h"
#include <QSettings>

class PortPageStyle
{
public:
    explicit PortPageStyle();
    void setStyle(PortPage* page);

private:
    QSettings settings;
};

