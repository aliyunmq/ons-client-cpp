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
#include <system_error>
#include <thread>

#include "AwaitPullCallback.h"
#include "MessageAccessor.h"
#include "rocketmq/ErrorCode.h"
#include "rocketmq/MQClientException.h"
#include "rocketmq/MQMessageExt.h"
#include "rocketmq/PullResult.h"
#include "gtest/gtest.h"

ROCKETMQ_NAMESPACE_BEGIN

TEST(AwaitPullCallbackTest, testOnSuccess) {
  std::vector<MQMessageExt> messages;
  PullResult pull_result(0, 32, 32, messages);
  AwaitPullCallback callback(pull_result);
  EXPECT_FALSE(callback.isCompleted());

  std::string topic{"Test"};
  int total = 32;
  int min = 32;
  int max = 128;
  int next = 64;
  auto task = [&]() {
    std::vector<MQMessageExt> msgs;
    for (int i = 0; i < total; i++) {
      MQMessageExt msg;
      msg.setTopic(topic);
      MessageAccessor::setQueueOffset(msg, i);
      msgs.emplace_back(msg);
    }
    PullResult result(32, 128, 64, msgs);
    callback.onSuccess(result);
  };

  std::thread t(task);

  callback.await();
  EXPECT_TRUE(callback.isCompleted());
  EXPECT_EQ(pull_result.messages().size(), total);
  EXPECT_EQ(pull_result.min(), min);
  EXPECT_EQ(pull_result.max(), max);
  EXPECT_EQ(pull_result.next(), next);

  if (t.joinable()) {
    t.join();
  }
}

TEST(AwaitPullCallbackTest, testOnFailure) {
  std::vector<MQMessageExt> messages;
  PullResult pull_result(0, 32, 32, messages);
  AwaitPullCallback callback(pull_result);
  std::string topic{"Test"};
  auto task = [&]() {
    std::error_code ec = ErrorCode::NotImplemented;
    callback.onFailure(ec);
  };

  std::thread t(task);

  callback.await();
  EXPECT_TRUE(callback.isCompleted());
  EXPECT_TRUE(callback.hasFailure());
  if (t.joinable()) {
    t.join();
  }
}

ROCKETMQ_NAMESPACE_END