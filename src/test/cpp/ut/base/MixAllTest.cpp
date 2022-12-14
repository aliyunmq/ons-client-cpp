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
#include "MixAll.h"
#include "rocketmq/MQMessage.h"
#include "rocketmq/RocketMQ.h"
#include "gtest/gtest.h"

ROCKETMQ_NAMESPACE_BEGIN
class MixAllTest : public testing::Test {
public:
  static std::string toUpperCase(const std::string& s) {
    std::string result;
    for (const char& c : s) {
      if ('a' <= c && 'z' >= c) {
        result.push_back(static_cast<char>('A' + (c - 'a')));
      } else {
        result.push_back(c);
      }
    }
    return result;
  }
};

TEST_F(MixAllTest, testValidate_empty_topic) {
  MQMessage message;
  ASSERT_FALSE(MixAll::validate(message));
}

TEST_F(MixAllTest, testValidate_normal_topic) {
  MQMessage message("T_abc-123", "sample_body");
  ASSERT_TRUE(MixAll::validate(message));
}

TEST_F(MixAllTest, testValidate_topic_too_long) {

  std::string topic("T");
  for (int i = 0;; ++i) {
    topic.append(std::to_string(i));
    if (topic.length() > 64) {
      break;
    }
  }
  MQMessage message(topic, "sample_body");
  ASSERT_FALSE(MixAll::validate(message));
}

TEST_F(MixAllTest, testValidate_body_too_large) {
  std::string topic("TestTopic");
  std::string body;

  body.reserve(MixAll::MAX_MESSAGE_BODY_SIZE + 1);
  for (uint32_t i = 0; i <= MixAll::MAX_MESSAGE_BODY_SIZE; ++i) {
    body.append("a");
  }
  ASSERT_FALSE(MixAll::validate(MQMessage(topic, body)));
}

TEST_F(MixAllTest, testRandom) {
  uint32_t left = 1;
  uint32_t right = 100;
  uint32_t random_number = MixAll::random(left, right);
  EXPECT_TRUE(random_number >= left && random_number <= right);
}

TEST_F(MixAllTest, testHex) {
  const char* data = "abc";
  std::string hex = MixAll::hex(data, strlen(data));
  std::vector<uint8_t> bin;
  EXPECT_TRUE(MixAll::hexToBinary(hex, bin));
  EXPECT_EQ(hex, MixAll::hex(bin.data(), bin.size()));
}

TEST_F(MixAllTest, testMD5) {
  std::string data("abc");
  std::string digest;
  bool success = MixAll::md5(data, digest);
  std::string expect("900150983CD24FB0D6963F7D28E17F72");
  EXPECT_TRUE(success);
  EXPECT_EQ(digest, expect);
}

TEST_F(MixAllTest, testSHA1) {
  std::string data("abc");
  std::string digest;
  bool ok = MixAll::sha1(data, digest);
  EXPECT_TRUE(ok);
  std::string expect("a9993e364706816aba3e25717850c26c9cd0d89d");
  EXPECT_EQ(digest, toUpperCase(expect));
}

TEST_F(MixAllTest, testCrc32) {
  std::string data("abc");
  std::string digest;
  bool success = MixAll::crc32(data, digest);
  EXPECT_TRUE(success);
  std::cout << digest << std::endl;
}

TEST_F(MixAllTest, testIsIpv4) {
  const char* ip = "8.8.8.8";
  const char* host = "www.taobao.com";
  EXPECT_TRUE(MixAll::isIPv4(ip));
  EXPECT_FALSE(MixAll::isIPv4(host));
}

ROCKETMQ_NAMESPACE_END