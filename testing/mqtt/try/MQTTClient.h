 /*******************************************************************************
  * Copyright (c) 2009, 2018 IBM Corp.
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
  *    Ian Craggs, Allan Stockdill-Mander - SSL updates
  *    Ian Craggs - multiple server connection support
  *    Ian Craggs - MQTT 3.1.1 support
  *    Ian Craggs - remove const from eyecatchers #168
  *******************************************************************************/
 
 /*
 */
 #if !defined(MQTTCLIENT_H)
 #define MQTTCLIENT_H
 
 #if defined(__cplusplus)
  extern "C" {
 #endif
 
 #if defined(WIN32) || defined(WIN64)
   #define DLLImport __declspec(dllimport)
   #define DLLExport __declspec(dllexport)
 #else
   #define DLLImport extern
   #define DLLExport __attribute__ ((visibility ("default")))
 #endif
 
 #include <stdio.h>
 /*
 */
 
 #include "MQTTProperties.h"
 #include "MQTTReasonCodes.h"
 #include "MQTTSubscribeOpts.h"
 #if !defined(NO_PERSISTENCE)
 #include "MQTTClientPersistence.h"
 #endif
 
 #define MQTTCLIENT_SUCCESS 0
 
 #define MQTTCLIENT_FAILURE -1
 
 /* error code -2 is MQTTCLIENT_PERSISTENCE_ERROR */
 
 #define MQTTCLIENT_DISCONNECTED -3
 
 #define MQTTCLIENT_MAX_MESSAGES_INFLIGHT -4
 
 #define MQTTCLIENT_BAD_UTF8_STRING -5
 
 #define MQTTCLIENT_NULL_PARAMETER -6
 
 #define MQTTCLIENT_TOPICNAME_TRUNCATED -7
 
 #define MQTTCLIENT_BAD_STRUCTURE -8
 
 #define MQTTCLIENT_BAD_QOS -9
 
 #define MQTTCLIENT_SSL_NOT_SUPPORTED -10
 
  #define MQTTCLIENT_BAD_MQTT_VERSION -11
 
 #define MQTTCLIENT_BAD_PROTOCOL -14
 
  #define MQTTCLIENT_BAD_MQTT_OPTION -15
 
  #define MQTTCLIENT_WRONG_MQTT_VERSION -16
 
 
 #define MQTTVERSION_DEFAULT 0
 
 #define MQTTVERSION_3_1 3
 
 #define MQTTVERSION_3_1_1 4
 
  #define MQTTVERSION_5 5
 
 #define MQTT_BAD_SUBSCRIBE 0x80
 
 typedef struct
 {
         char struct_id[4];
         int struct_version;
         int do_openssl_init;
 } MQTTClient_init_options;
 
 #define MQTTClient_init_options_initializer { {'M', 'Q', 'T', 'G'}, 0, 0 }
 
 DLLExport void MQTTClient_global_init(MQTTClient_init_options* inits);
 
 typedef void* MQTTClient;
 typedef int MQTTClient_deliveryToken;
 typedef int MQTTClient_token;
 
 typedef struct
 {
         char struct_id[4];
         int struct_version;
         int payloadlen;
         void* payload;
         int qos;
         int retained;
         int dup;
         int msgid;
         MQTTProperties properties;
 } MQTTClient_message;
 
 #define MQTTClient_message_initializer { {'M', 'Q', 'T', 'M'}, 1, 0, NULL, 0, 0, 0, 0, MQTTProperties_initializer }
 
 typedef int MQTTClient_messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message);
 
 typedef void MQTTClient_deliveryComplete(void* context, MQTTClient_deliveryToken dt);
 
 typedef void MQTTClient_connectionLost(void* context, char* cause);
 
 DLLExport int MQTTClient_setCallbacks(MQTTClient handle, void* context, MQTTClient_connectionLost* cl,
                                                                         MQTTClient_messageArrived* ma, MQTTClient_deliveryComplete* dc);
 
 
 typedef void MQTTClient_disconnected(void* context, MQTTProperties* properties,
                 enum MQTTReasonCodes reasonCode);
 
 DLLExport int MQTTClient_setDisconnected(MQTTClient handle, void* context, MQTTClient_disconnected* co);
 
 typedef void MQTTClient_published(void* context, int dt, int packet_type, MQTTProperties* properties,
                 enum MQTTReasonCodes reasonCode);
 
 DLLExport int MQTTClient_setPublished(MQTTClient handle, void* context, MQTTClient_published* co);
 
 DLLExport int MQTTClient_create(MQTTClient* handle, const char* serverURI, const char* clientId,
                 int persistence_type, void* persistence_context);
 
 typedef struct
 {
         char struct_id[4];
         int struct_version;
         int MQTTVersion;
 } MQTTClient_createOptions;
 
 #define MQTTClient_createOptions_initializer { {'M', 'Q', 'C', 'O'}, MQTTVERSION_DEFAULT }
 
 DLLExport int MQTTClient_createWithOptions(MQTTClient* handle, const char* serverURI, const char* clientId,
                 int persistence_type, void* persistence_context, MQTTClient_createOptions* options);
 
 typedef struct
 {
         char struct_id[4];
         int struct_version;
         const char* topicName;
         const char* message;
         int retained;
         int qos;
         struct
         {
                 int len;            
                 const void* data;  
         } payload;
 } MQTTClient_willOptions;
 
 #define MQTTClient_willOptions_initializer { {'M', 'Q', 'T', 'W'}, 1, NULL, NULL, 0, 0, {0, NULL} }
 
 #define MQTT_SSL_VERSION_DEFAULT 0
 #define MQTT_SSL_VERSION_TLS_1_0 1
 #define MQTT_SSL_VERSION_TLS_1_1 2
 #define MQTT_SSL_VERSION_TLS_1_2 3
 
 typedef struct
 {
         char struct_id[4];
         int struct_version;
 
         const char* trustStore;
 
         const char* keyStore;
 
         const char* privateKey;
         const char* privateKeyPassword;
 
         const char* enabledCipherSuites;
 
     int enableServerCertAuth;
 
     int sslVersion;
 
     int verify;
 
         const char* CApath;
 
     int (*ssl_error_cb) (const char *str, size_t len, void *u);
 
     void* ssl_error_context;
 
 } MQTTClient_SSLOptions;
 
 #define MQTTClient_SSLOptions_initializer { {'M', 'Q', 'T', 'S'}, 3, NULL, NULL, NULL, NULL, NULL, 1, MQTT_SSL_VERSION_DEFAULT, 0, NULL, NULL, NULL }
 
 typedef struct
 {
         char struct_id[4];
         int struct_version;
         int keepAliveInterval;
         int cleansession;
         int reliable;
         MQTTClient_willOptions* will;
         const char* username;
         const char* password;
         int connectTimeout;
         int retryInterval;
         MQTTClient_SSLOptions* ssl;
         int serverURIcount;
         char* const* serverURIs;
         int MQTTVersion;
         struct
         {
                 const char* serverURI;     
                 int MQTTVersion;     
                 int sessionPresent;  
         } returned;
         struct
         {
                 int len;           
                 const void* data;  
         } binarypwd;
         int maxInflightMessages;
         /*
          * MQTT V5 clean start flag.  Only clears state at the beginning of the session.
          */
         int cleanstart;
 } MQTTClient_connectOptions;
 
 #define MQTTClient_connectOptions_initializer { {'M', 'Q', 'T', 'C'}, 6, 60, 1, 1, NULL, NULL, NULL, 30, 0, NULL, 0, NULL, MQTTVERSION_DEFAULT, {NULL, 0, 0}, {0, NULL}, -1, 0}
 
 #define MQTTClient_connectOptions_initializer5 { {'M', 'Q', 'T', 'C'}, 6, 60, 0, 1, NULL, NULL, NULL, 30, 0, NULL, 0, NULL, MQTTVERSION_5, {NULL, 0, 0}, {0, NULL}, -1, 1}
 
 typedef struct
 {
         const char* name;
         const char* value;
 } MQTTClient_nameValue;
 
 DLLExport MQTTClient_nameValue* MQTTClient_getVersionInfo(void);
 
 DLLExport int MQTTClient_connect(MQTTClient handle, MQTTClient_connectOptions* options);
 
 
 typedef struct MQTTResponse
 {
         int version;
         enum MQTTReasonCodes reasonCode;
         int reasonCodeCount;                   /* used for subscribeMany5 and unsubscribeMany5 */
         enum MQTTReasonCodes* reasonCodes;  /* used for subscribeMany5 and unsubscribeMany5 */
         MQTTProperties* properties;         /* optional */
 } MQTTResponse;
 
 #define MQTTResponse_initializer {1, MQTTREASONCODE_SUCCESS, 0, NULL, NULL}
 
 DLLExport void MQTTResponse_free(MQTTResponse response);
 
 DLLExport MQTTResponse MQTTClient_connect5(MQTTClient handle, MQTTClient_connectOptions* options,
                 MQTTProperties* connectProperties, MQTTProperties* willProperties);
 
 DLLExport int MQTTClient_disconnect(MQTTClient handle, int timeout);
 
 DLLExport int MQTTClient_disconnect5(MQTTClient handle, int timeout, enum MQTTReasonCodes reason, MQTTProperties* props);
 
 DLLExport int MQTTClient_isConnected(MQTTClient handle);
 
 
 /* Subscribe is synchronous.  QoS list parameter is changed on return to granted QoSs.
    Returns return code, MQTTCLIENT_SUCCESS == success, non-zero some sort of error (TBD) */
 
 DLLExport int MQTTClient_subscribe(MQTTClient handle, const char* topic, int qos);
 
 
 DLLExport MQTTResponse MQTTClient_subscribe5(MQTTClient handle, const char* topic, int qos,
                 MQTTSubscribe_options* opts, MQTTProperties* props);
 
 DLLExport int MQTTClient_subscribeMany(MQTTClient handle, int count, char* const* topic, int* qos);
 
 DLLExport MQTTResponse MQTTClient_subscribeMany5(MQTTClient handle, int count, char* const* topic,
                 int* qos, MQTTSubscribe_options* opts, MQTTProperties* props);
 
 DLLExport int MQTTClient_unsubscribe(MQTTClient handle, const char* topic);
 
 DLLExport MQTTResponse MQTTClient_unsubscribe5(MQTTClient handle, const char* topic, MQTTProperties* props);
 
 DLLExport int MQTTClient_unsubscribeMany(MQTTClient handle, int count, char* const* topic);
 
 DLLExport MQTTResponse MQTTClient_unsubscribeMany5(MQTTClient handle, int count, char* const* topic, MQTTProperties* props);
 
 DLLExport int MQTTClient_publish(MQTTClient handle, const char* topicName, int payloadlen, const void* payload, int qos, int retained,
                                                                                                                                  MQTTClient_deliveryToken* dt);
 
 DLLExport MQTTResponse MQTTClient_publish5(MQTTClient handle, const char* topicName, int payloadlen, const void* payload,
                 int qos, int retained, MQTTProperties* properties, MQTTClient_deliveryToken* dt);
 DLLExport int MQTTClient_publishMessage(MQTTClient handle, const char* topicName, MQTTClient_message* msg, MQTTClient_deliveryToken* dt);
 
 
 DLLExport MQTTResponse MQTTClient_publishMessage5(MQTTClient handle, const char* topicName, MQTTClient_message* msg,
                 MQTTClient_deliveryToken* dt);
 
 DLLExport int MQTTClient_waitForCompletion(MQTTClient handle, MQTTClient_deliveryToken dt, unsigned long timeout);
 
 
 DLLExport int MQTTClient_getPendingDeliveryTokens(MQTTClient handle, MQTTClient_deliveryToken **tokens);
 
 DLLExport void MQTTClient_yield(void);
 
 DLLExport int MQTTClient_receive(MQTTClient handle, char** topicName, int* topicLen, MQTTClient_message** message,
                 unsigned long timeout);
 
 DLLExport void MQTTClient_freeMessage(MQTTClient_message** msg);
 
 DLLExport void MQTTClient_free(void* ptr);
 
 DLLExport void MQTTClient_destroy(MQTTClient* handle);
 
 
 enum MQTTCLIENT_TRACE_LEVELS
 {
         MQTTCLIENT_TRACE_MAXIMUM = 1,
         MQTTCLIENT_TRACE_MEDIUM,
         MQTTCLIENT_TRACE_MINIMUM,
         MQTTCLIENT_TRACE_PROTOCOL,
         MQTTCLIENT_TRACE_ERROR,
         MQTTCLIENT_TRACE_SEVERE,
         MQTTCLIENT_TRACE_FATAL,
 };
 
 
 DLLExport void MQTTClient_setTraceLevel(enum MQTTCLIENT_TRACE_LEVELS level);
 
 
 typedef void MQTTClient_traceCallback(enum MQTTCLIENT_TRACE_LEVELS level, char* message);
 
 DLLExport void MQTTClient_setTraceCallback(MQTTClient_traceCallback* callback);
 
 DLLExport const char* MQTTClient_strerror(int code);
 
 #ifdef __cplusplus
      }
 #endif
 
 #endif