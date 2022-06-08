/*******************************************************************************
  * Copyright (c) 2018 IBM Corp.
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
 
 #if !defined(SUBOPTS_H)
 #define SUBOPTS_H
 
 typedef struct MQTTSubscribe_options
 {
         char struct_id[4];
         int struct_version;
         unsigned char noLocal;
         unsigned char retainAsPublished;
         unsigned char retainHandling;
 } MQTTSubscribe_options;
 
 #define MQTTSubscribe_options_initializer { {'M', 'Q', 'S', 'O'}, 0, 0, 0, 0 }
 
 #endif