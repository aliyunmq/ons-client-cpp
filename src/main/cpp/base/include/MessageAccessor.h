/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "rocketmq/MQMessageExt.h"

#include "Protocol.h"

ROCKETMQ_NAMESPACE_BEGIN

class MessageAccessor {

public:
  static void setMessageId(MQMessageExt& message, std::string message_id);

  static void setBornTimestamp(MQMessageExt& message, absl::Time born_timestamp);

  static void setStoreTimestamp(MQMessageExt& message, absl::Time store_timestamp);

  static void setQueueId(MQMessageExt& message, int32_t queue_id);

  static void setQueueOffset(MQMessageExt& message, int64_t queue_offset);

  static void setBornHost(MQMessageExt& message, std::string born_host);

  static void setStoreHost(MQMessageExt& message, std::string store_host);

  static void setDeliveryTimestamp(MQMessageExt& message, absl::Time delivery_timestamp);

  static void setDeliveryAttempt(MQMessageExt& message, int32_t attempt_times);

  static void setDecodedTimestamp(MQMessageExt& message, absl::Time decode_timestamp);
  static absl::Time decodedTimestamp(const MQMessageExt& message);

  static void setInvisiblePeriod(MQMessageExt& message, absl::Duration invisible_period);

  static void setReceiptHandle(MQMessageExt& message, std::string receipt_handle);
  static void setTraceContext(MQMessageExt& message, std::string trace_context);

  static void setMessageType(MQMessage& message, MessageType message_type);

  static void setTargetEndpoint(MQMessage& message, const std::string& target_endpoint);

  static const std::string& targetEndpoint(const MQMessage& message);
};

ROCKETMQ_NAMESPACE_END