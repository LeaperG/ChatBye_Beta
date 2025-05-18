#include "server_message_dictionary.h"

ServerMessageDictionary::ServerMessageDictionary()
{
    initializeMessages();
}

QString ServerMessageDictionary::getMessage(int code) const
{
    return messages.value(code, "Неизвестный код ошибки");
}

void ServerMessageDictionary::addMessage(int code, const QString &message)
{
    messages[code] = message;
}

void ServerMessageDictionary::initializeMessages()
{
    messages[100] = "Соединение установлено";
    messages[200] = "Операция выполнена успешно";
    messages[400] = "Ошибка запроса";
    messages[500] = "Внутренняя ошибка сервера";
    //messages[600] = "Your name is now set to";
    //messages[600] = "Установлено новое имя";
    messages[600] = "Добро пожаловать!";
    //messages[700] = "Подключился к чату";
    messages[800] = "Client disconnected";
    messages[900] = "Вы сменили имя";
    messages[1000] = "Сервер завершает работу. Переподключаемся...";
    messages[1100] = "теперь известен как";
    messages[1200] = "Please set your name first using /name <YourName>";
    messages[1300] = "Server shutting down";
    messages[1400] = "Для раздлеления текста";
}
