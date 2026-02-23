#include <stdint.h>
#include <stddef.h>

extern void report_packet(const char *text, uint32_t len);
extern void report_packet_hex(const uint8_t *data, uint32_t len);

static uint8_t g_buf[4096];
static uint32_t g_len = 0;

void reset_parser(void) {
  g_len = 0;
}

void parse_bytes(const uint8_t *data, uint32_t len) {

}
