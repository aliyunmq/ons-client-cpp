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
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/synchronization/mutex.h"
#include "grpc/grpc.h"
#include "gtest/gtest.h"

#include "HttpClientMock.h"
#include "TopAddressing.h"

ROCKETMQ_NAMESPACE_BEGIN

class TopAddressingTest : public testing::Test {
public:
  void SetUp() override {
    grpc_init();
    top_addressing_ = absl::make_unique<TopAddressing>();
    auto http_client_ = absl::make_unique<testing::NiceMock<HttpClientMock>>();
    auto mock_get =
        [](HttpProtocol protocol, const std::string& host, std::uint16_t port, const std::string& path,
           const std::function<void(int, const std::multimap<std::string, std::string>&, const std::string&)>& cb) {
          std::multimap<std::string, std::string> headers;
          std::string body("10.0.0.1:9876");
          cb(200, headers, body);
        };
    ON_CALL(*http_client_, get).WillByDefault(testing::Invoke(mock_get));
    top_addressing_->injectHttpClient(std::move(http_client_));
  }

  void TearDown() override {
    grpc_shutdown();
  }

protected:
  std::unique_ptr<TopAddressing> top_addressing_;
};

TEST_F(TopAddressingTest, testFetchNameServerAddresses) {
  bool completed = false;
  absl::Mutex mtx;
  absl::CondVar cv;

  auto callback = [&](bool ok, const std::vector<std::string>& name_server_list) {
    absl::MutexLock lk(&mtx);
    completed = true;
    cv.SignalAll();
  };
  top_addressing_->fetchNameServerAddresses(callback);
  {
    absl::MutexLock lk(&mtx);
    cv.WaitWithDeadline(&mtx, absl::Now() + absl::Seconds(3));
  }
  EXPECT_TRUE(completed);
}

ROCKETMQ_NAMESPACE_END