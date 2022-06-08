/*******************************************************************************
  * Copyright (c) 2017, 2018 IBM Corp.
  *
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * and Eclipse Distribution License v1.0 which accompany this distribution.
  *
  * The Eclipse Public License is available at
  *    http://www.eclipse.org/legal/epl-v10.html
  * and the Eclipse Distribution License is available at
  *   http://www.eclipse.org/org/documents/edl-v10.php.
  *
  * Contributors:
  *    Ian Craggs - initial API and implementation and/or initial documentation
  *******************************************************************************/
 
 #if !defined(MQTTPROPERTIES_H)
 #define MQTTPROPERTIES_H
 
 #define MQTT_INVALID_PROPERTY_ID -2
 
 enum MQTTPropertyCodes {
   MQTTPROPERTY_CODE_PAYLOAD_FORMAT_INDICATOR = 1,  
   MQTTPROPERTY_CODE_MESSAGE_EXPIRY_INTERVAL = 2,   
   MQTTPROPERTY_CODE_CONTENT_TYPE = 3,              
   MQTTPROPERTY_CODE_RESPONSE_TOPIC = 8,            
   MQTTPROPERTY_CODE_CORRELATION_DATA = 9,          
   MQTTPROPERTY_CODE_SUBSCRIPTION_IDENTIFIER = 11,  
   MQTTPROPERTY_CODE_SESSION_EXPIRY_INTERVAL = 17,  
   MQTTPROPERTY_CODE_ASSIGNED_CLIENT_IDENTIFER = 18,
   MQTTPROPERTY_CODE_SERVER_KEEP_ALIVE = 19,        
   MQTTPROPERTY_CODE_AUTHENTICATION_METHOD = 21,    
   MQTTPROPERTY_CODE_AUTHENTICATION_DATA = 22,      
   MQTTPROPERTY_CODE_REQUEST_PROBLEM_INFORMATION = 23,
   MQTTPROPERTY_CODE_WILL_DELAY_INTERVAL = 24,      
   MQTTPROPERTY_CODE_REQUEST_RESPONSE_INFORMATION = 25,
   MQTTPROPERTY_CODE_RESPONSE_INFORMATION = 26,     
   MQTTPROPERTY_CODE_SERVER_REFERENCE = 28,         
   MQTTPROPERTY_CODE_REASON_STRING = 31,            
   MQTTPROPERTY_CODE_RECEIVE_MAXIMUM = 33,          
   MQTTPROPERTY_CODE_TOPIC_ALIAS_MAXIMUM = 34,      
   MQTTPROPERTY_CODE_TOPIC_ALIAS = 35,              
   MQTTPROPERTY_CODE_MAXIMUM_QOS = 36,              
   MQTTPROPERTY_CODE_RETAIN_AVAILABLE = 37,         
   MQTTPROPERTY_CODE_USER_PROPERTY = 38,            
   MQTTPROPERTY_CODE_MAXIMUM_PACKET_SIZE = 39,      
   MQTTPROPERTY_CODE_WILDCARD_SUBSCRIPTION_AVAILABLE = 40,
   MQTTPROPERTY_CODE_SUBSCRIPTION_IDENTIFIERS_AVAILABLE = 41,
   MQTTPROPERTY_CODE_SHARED_SUBSCRIPTION_AVAILABLE = 42
 };
 
 #if defined(WIN32) || defined(WIN64)
   #define DLLImport __declspec(dllimport)
   #define DLLExport __declspec(dllexport)
 #else
   #define DLLImport extern
   #define DLLExport __attribute__ ((visibility ("default")))
 #endif
 
 DLLExport const char* MQTTPropertyName(enum MQTTPropertyCodes value);
 
 enum MQTTPropertyTypes {
   MQTTPROPERTY_TYPE_BYTE,
   MQTTPROPERTY_TYPE_TWO_BYTE_INTEGER,
   MQTTPROPERTY_TYPE_FOUR_BYTE_INTEGER,
   MQTTPROPERTY_TYPE_VARIABLE_BYTE_INTEGER,
   MQTTPROPERTY_TYPE_BINARY_DATA,
   MQTTPROPERTY_TYPE_UTF_8_ENCODED_STRING,
   MQTTPROPERTY_TYPE_UTF_8_STRING_PAIR
 };
 
 DLLExport int MQTTProperty_getType(enum MQTTPropertyCodes value);
 
 typedef struct
 {
         int len; 
         char* data; 
 } MQTTLenString;
 
 
 typedef struct
 {
   enum MQTTPropertyCodes identifier; 
   union {
     char byte;       
     short integer2;  
     int integer4;    
     struct {
       MQTTLenString data;  
       MQTTLenString value; 
     };
   } value;
 } MQTTProperty;
 
 typedef struct MQTTProperties
 {
   int count;     
   int max_count; 
   int length;    
   MQTTProperty *array;  
 } MQTTProperties;
 
 #define MQTTProperties_initializer {0, 0, 0, NULL}
 
 int MQTTProperties_len(MQTTProperties* props);
 
 DLLExport int MQTTProperties_add(MQTTProperties* props, const MQTTProperty* prop);
 
 int MQTTProperties_write(char** pptr, const MQTTProperties* properties);
 
 int MQTTProperties_read(MQTTProperties* properties, char** pptr, char* enddata);
 
 DLLExport void MQTTProperties_free(MQTTProperties* properties);
 
 DLLExport MQTTProperties MQTTProperties_copy(const MQTTProperties* props);
 
 DLLExport int MQTTProperties_hasProperty(MQTTProperties *props, enum MQTTPropertyCodes propid);
 
 DLLExport int MQTTProperties_propertyCount(MQTTProperties *props, enum MQTTPropertyCodes propid);
 
 DLLExport int MQTTProperties_getNumericValue(MQTTProperties *props, enum MQTTPropertyCodes propid);
 
 DLLExport int MQTTProperties_getNumericValueAt(MQTTProperties *props, enum MQTTPropertyCodes propid, int index);
 
 DLLExport MQTTProperty* MQTTProperties_getProperty(MQTTProperties *props, enum MQTTPropertyCodes propid);
 
 DLLExport MQTTProperty* MQTTProperties_getPropertyAt(MQTTProperties *props, enum MQTTPropertyCodes propid, int index);
 
 #endif /* MQTTPROPERTIES_H */