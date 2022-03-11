
#include "Replica.h"
// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

// for JsonRPCCPP
#include <iostream>
#include "minigfs_server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

// ecs251
#include "Core.h"
#include "Directory.h"
#include "Replica.h"
#include "Shadow_Directory.h"
#include <time.h>

Chunk::Chunk()
{
  this->data = "";
}

Chunk::Chunk(std::string arg_data)
{
  this->data = arg_data;
}

Json::Value *
Chunk::dumpJ()
{
  Json::Value * result_ptr = new Json::Value();

  if (this->data != "")
    {
      (*result_ptr)["data"] = this->data;
    }

  return result_ptr;
}

bool
Chunk::Jdump(Json::Value *input_json_ptr)
{
  if ((input_json_ptr == NULL) ||
      ((*input_json_ptr).isNull() == true) ||
      ((*input_json_ptr).isObject() != true))
    {
      return false;
    }

  if ((((*input_json_ptr)["data"]).isNull() == true) ||
      (((*input_json_ptr)["data"]).isString() != true))
    {
      return false;
    }

  this->data = ((*input_json_ptr)["data"]).asString();

  return true;
}

Replica::Replica
(std::string core_arg_host_url, std::string core_arg_owner_vsID,
 std::string core_arg_class_id, std::string core_arg_object_id)
  : Core { core_arg_host_url, core_arg_owner_vsID,
    core_arg_class_id, core_arg_object_id }
{
  std::cout << "a replica has been created" << std::endl;
}

Replica::Replica
(std::string core_arg_host_url, std::string core_arg_owner_vsID,
 std::string core_arg_class_id, std::string core_arg_object_id,
 std::string arg_chunk1_data, std::string arg_chunk2_data)
  : Core { core_arg_host_url, core_arg_owner_vsID,
    core_arg_class_id, core_arg_object_id }
{
  (this->committed_data_chunk1).data = arg_chunk1_data;
  (this->committed_data_chunk2).data = arg_chunk2_data;
}

Json::Value
Replica::CommitAbort
(std::string arg_name, std::string arg_fhandle, std::string arg_chunk_index,
 std::string arg_commitorabort)
{
  Json::Value result;
  //
  if (arg_commitorabort == "commit"){
      if (arg_chunk_index == "0"){
          this->committed_data_chunk1.data = this->uncommitted_data_chunk1.data;
      } else {
          this->committed_data_chunk2.data = this->uncommitted_data_chunk2.data;
      }
      result["status"] = "committed";
      result["Chunk1_data"] = (this->committed_data_chunk1).data;
      result["Chunk2_data"] = (this->committed_data_chunk2).data;
  }

//  {(this->committed_data).data  = (this->uncommitted_data).data;
//    result["status"] = "committed";
//    result["data"] = (this->committed_data).data;
//  }
  return result;
}

Json::Value
Replica::PushChunk2Replica
(std::string arg_name, std::string arg_fhandle, std::string arg_chunk_index, std::string arg_chunk)
{
  Json::Value result;

  std::string chunk_index = arg_chunk_index;
  if (chunk_index == "0"){
      this->uncommitted_data_chunk1.data = arg_chunk;
  } else{
      this->uncommitted_data_chunk2.data = arg_chunk;
  }

//  (this->uncommitted_data).data = arg_chunk;
  result["vote"] = "commit";
  return result;
}


Json::Value *
Replica::dumpJ()
{
  Json::Value * result_ptr = new Json::Value();

  if (this->name != "")
    {
      (*result_ptr)["name"] = this->name;
    }

  if (this->fhandle != "")
    {
      (*result_ptr)["fhandle"] = this->fhandle;
    }

  if (this->chunk_index != "")
    {
      (*result_ptr)["chunk_index"] = this->chunk_index;
    }

        if (this->committed_data_chunk1.data != "")
    {
        (*result_ptr)["chunk1_data"] = this->committed_data_chunk1.data;
    } else {
        (*result_ptr)["chunk1_data"] = "empty";
    }

    if (this->committed_data_chunk1.data != "")
    {
        (*result_ptr)["chunk2_data"] = this->committed_data_chunk2.data;
    } else {
        (*result_ptr)["chunk2_data"] = "empty";
    }




  std::cout << " Replica DumpJ\n";
  return result_ptr;
}

bool
Replica::Jdump(Json::Value *input_json_ptr)
{
  if ((input_json_ptr == NULL) ||
      ((*input_json_ptr).isNull() == true) ||
      ((*input_json_ptr).isObject() != true))
    {
      return false;
    }

  if ((((*input_json_ptr)["name"]).isNull() == true) ||
      (((*input_json_ptr)["fhandle"]).isNull() == true) ||
      (((*input_json_ptr)["name"]).isString() != true) ||
      (((*input_json_ptr)["fhandle"]).isString() != true))
    {
      return false;
    }

  this->name    = ((*input_json_ptr)["name"]).asString();
  this->fhandle = ((*input_json_ptr)["fhandle"]).asString();
  return true;
}

