#ifndef JSONDATAPARSER_H
#define JSONDATAPARSER_H
 
#include <QVariant>
#include <QList>
#include <QMap>
#include <QString>
#include <QTextCodec>
 
extern "C"{
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <json.h>
#include <json_object.h>
};
 
class JsonDataParser
{
public:
    JsonDataParser();
    QVariant parse( QByteArray, unsigned short &status );
 
private:
    QList<QVariant> parseArray( struct json_object *, unsigned short &status );
    QMap<QString,QVariant> parseObject( struct json_object *, unsigned short &status );
    QVariant getData(struct json_object *, unsigned short & );
 
    QTextCodec *codec;
};
 
#endif // JSONDATAPARSER_H
