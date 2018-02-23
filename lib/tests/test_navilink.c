#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <math.h>
#include <stdlib.h>

#include "navilink.h"

void DumpHex(const void* data, size_t size)
{
  char ascii[17];
  size_t i, j;
  ascii[16] = '\0';
  for (i = 0; i < size; ++i) {
    printf("%02X ", ((unsigned char*)data)[i]);
    if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
      ascii[i % 16] = ((unsigned char*)data)[i];
    }
    else {
      ascii[i % 16] = '.';
    }
    if ((i + 1) % 8 == 0 || i + 1 == size) {
      printf(" ");
      if ((i + 1) % 16 == 0) {
        fprintf(stdout, "|  %s \n", ascii);
      }
      else if (i + 1 == size) {
        ascii[(i + 1) % 16] = '\0';
        if ((i + 1) % 16 <= 8) {
          fprintf(stdout, " ");
        }
        for (j = (i + 1) % 16; j < 16; ++j) {
          fprintf(stdout, "   ");
        }
        fprintf(stdout, "|  %s \n", ascii);
      }
    }
  }
}

int init_suite_success() { return 0; }
int init_suite_failure() { return -1; }
int clean_suite_success() { return 0; }
int clean_suite_failure() { return -1; }
/**
    As we assume test are runned sequentially,
    we can use the same buffer to transform data.
*/

/** Reimport private header */
uint16_t navilink_compute_checksum(const uint8_t* payloaddata, uint16_t size);

void test_packet_crc()
{
  // 1 byte payload
  uint8_t payload[4] = { 0xd6 };
  uint16_t crc = navilink_compute_checksum(&payload[0], 4);
  CU_ASSERT_EQUAL(crc, 0xd6);

  // Bigger payload
  uint8_t payload2[8] = { 0x28, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01 };
  crc = navilink_compute_checksum(&payload2[0], 8);
  CU_ASSERT_EQUAL(crc, 0x2a);
}

void assert_packet_content(const uint8_t* expected, const uint8_t* actual, size_t length)
{
  for (unsigned int i = 0; i < length; i++) {
    CU_ASSERT_EQUAL(expected[i], actual[i]);
  }
}

void test_packet_creation()
{
  NavilinkPacket packet;

  /* SYNC PACKET */
  uint8_t buffer[NAVILINK_MAX_PACKET_SIZE] = { 0 };
  uint8_t expected[] = { 0xA0, 0xA2, 0x01, 0x00, 0xd6, 0xd6, 0x00, 0xB0, 0xB3 };
  navilink_create_packet(&buffer[0], NAVILINK_PID_SYNC, NULL, 0);
  assert_packet_content(&expected[0], &buffer[0], sizeof(expected));

  /* QUERY */
  uint8_t expected_waypoint[] = { 0xA0, 0xA2, 0x08, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x2A, 0x00, 0xB0, 0xB3 };
  uint8_t query_waypoint_payload[] = { 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01 };
  navilink_create_packet(&buffer[0], NAVILINK_PID_QRY_WAYPOINTS, &query_waypoint_payload[0], 7);
  assert_packet_content(&expected_waypoint[0], &buffer[0], sizeof(expected_waypoint));

  /* QUERY Multiple */
  uint8_t expected_waypoint_mul[] = { 0xA0, 0xA2, 0x08, 0x00, 0x28, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x2C, 0x00, 0xB0, 0xB3 };
  uint8_t query_waypoint_payload_mul[] = { 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01 };
  navilink_create_packet(&buffer[0], NAVILINK_PID_QRY_WAYPOINTS, &query_waypoint_payload_mul[0], 7);
  assert_packet_content(&expected_waypoint_mul[0], &buffer[0], sizeof(expected_waypoint_mul));
}

int main(int argc, char** argv)
{

  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("Suite_success", init_suite_success, clean_suite_success);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite testProjectionsecant*/
  if (NULL == CU_add_test(pSuite, "Test Packet CRC", test_packet_crc) || NULL == CU_add_test(pSuite, "Test Packet Creation", test_packet_creation)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  // CU_automated_run_tests();

  CU_list_tests_to_file();

  /* Clean up registry and return */
  CU_cleanup_registry();
  return CU_get_error();

  return 0;
}