/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef __DA1469X_SLEEP_H
#define __DA1469X_SLEEP_H

#include <stdint.h>
#include <DA1469xAB.h>

#ifdef __cplusplus
extern "C" {
#endif

struct da1469x_sleep_config {
    bool enable_xtal_on_wakeup;
};

void da1469x_sleep_config(const struct da1469x_sleep_config *config);
int da1469x_sleep(void);
void da1469x_wakeup_handler(void);

int da1469x_enter_sleep(void);

#ifdef __cplusplus
}
#endif

#endif  /* __DA1469X_SLEEP_H */
