// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include <rapidjson/document.h>
#include <stddef.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/status.h"
#include "exec/olap_common.h"
#include "vec/exec/format/jni_reader.h"

namespace doris {

class RuntimeProfile;

class RuntimeState;

class SlotDescriptor;

namespace vectoried {

class Block;

} // namespace vectoried
struct TypeDescriptor;
} // namespace doris

namespace doris::vectorized {

/**
 *  Read hive-format file: rcbinary, rctext, sequencefile
 */
class HiveJNIReader : public JniReader {
    ENABLE_FACTORY_CREATOR(HiveJNIReader);

public:
    /**
     *  Call java side by jni to get table data
     */
    HiveJNIReader(RuntimeState* state, RuntimeProfile* profile, const TFileScanRangeParams& params,
                  const std::vector<SlotDescriptor*>& file_slot_descs, const TFileRangeDesc& range);

    ~HiveJNIReader() override;

    Status get_next_block(Block* block, size_t* read_rows, bool* eof) override;

    Status get_columns(std::unordered_map<std::string, TypeDescriptor>* name_to_type,
                       std::unordered_set<std::string>* missing_cols) override;

    Status init_fetch_table_reader(
            std::unordered_map<std::string, ColumnValueRangeType>* colname_to_value_range);

    TFileType::type get_file_type();

private:
    const TFileScanRangeParams _params;
    const TFileRangeDesc _range;
    std::string _column_names;
    std::string _column_types;
    std::unordered_map<std::string, ColumnValueRangeType>* _colname_to_value_range = nullptr;
};

} // namespace doris::vectorized