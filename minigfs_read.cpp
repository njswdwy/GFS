
// ecs251 miniGFS

#include <iostream>
#include "Shadow_Directory.h"
#include "Shadow_Replica.h"

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

using namespace jsonrpc;
using namespace std;

int
main()
{
  Shadow_Directory gfs_master 
  { "http://127.0.0.1:8384", "1234567890", "Directory", "00000000" };

  Json::Value result, result_P, result_A, result_B;

  //Step1-2
  result = gfs_master.ObtainChunkURL("my_ecs251_file", "00000002", "0");
  while((result["status"]).asString() != "URLpassed"){
    result = gfs_master.ObtainChunkURL("my_ecs251_file", "00000002", "0");
  }

  std::string url_primary = (result["primary"]).asString();
  Shadow_Replica gfs_primary
  { url_primary, "1234567890", "Replica", "00000001" };

  std::string url_secondary_A = (result["secondary_A"]).asString();
  Shadow_Replica gfs_secondary_A
  { url_secondary_A, "1234567890", "Replica", "00000002" };

  std::string url_secondary_B = (result["secondary_B"]).asString();
  Shadow_Replica gfs_secondary_B
  { url_secondary_B, "1234567890", "Replica", "00000003" };

  // Json::Value reading_test = gfs_primary.dumpJ();

  std::string my_chunk_data = { "ecs251 data" };

    std::cout << "---------- MiniGFS_Reading: Read data from all replicas----------" << std::endl;
    Json::Value reading_primary = gfs_primary.dumpJ();
    Json::Value reading_secondA = gfs_secondary_A.dumpJ();
    Json::Value reading_secondB = gfs_secondary_B.dumpJ();

  return 0;
}
