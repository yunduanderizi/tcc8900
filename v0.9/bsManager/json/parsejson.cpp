#include <QtDebug>
 
#include "JsonDataParser.h"
 
JsonDataParser::JsonDataParser()
{
     codec = QTextCodec::codecForName("UTF-8");
}
 
QVariant JsonDataParser::parse(QByteArray sdata,unsigned short &status ){
    QVariant data;
    struct json_object *obj;
    obj = json_tokener_parse(sdata.data() );
 
    data = getData(obj,status);
    return data;
}
 
QList<QVariant> JsonDataParser::parseArray(struct json_object *array, unsigned short &status ){
    QList<QVariant> data;
    struct json_object *t_obj;
 
//qDebug()<<"Found an array and inserted something ";
    for( int i=0 ; i<json_object_array_length(array) ; i++){
        t_obj = json_object_array_get_idx(array,i);
 
        data.append( getData(t_obj, status ) );
    }
 
    return data;
}
 
QMap<QString,QVariant> JsonDataParser::parseObject(struct json_object *obj, unsigned short &status ){
    QMap<QString, QVariant> data;
 
   // struct json_object *key_obj, *val_obj;
//qDebug()<<"Found an object and inserted something ";
 
    json_object_object_foreach( obj,key_obj, val_obj ){
        data.insert( QString( key_obj ), getData(val_obj,status) );
    }
    return data;
}
 
QVariant JsonDataParser:: getData(json_object *obj, unsigned short &status){
    QVariant data;
    switch(json_object_get_type(obj)){
            case json_type_object:
                data=( ( parseObject(obj,status)) );
            break;
 
            case json_type_array:
                data=( ( parseArray(obj,status) ) );
            break;
 
            case json_type_string:
               // qDebug()<<"Found string: "<< json_object_get_string(obj);
                data=( QVariant( codec->toUnicode( json_object_get_string(obj) ) ) );
            break;
 
            case json_type_boolean:
                data=( QVariant( json_object_get_boolean(obj) ) );
            break;
 
            case json_type_int:
                data=( QVariant( json_object_get_int(obj)));
            break;
 
            case json_type_double:
                data= QVariant( json_object_get_double(obj) );
            break;
 
            default:
                data = QVariant();
            break;
    }
 
    return data;
}