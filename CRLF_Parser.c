// Simple CRLF line parser in C (callback style)
//
// JS <-> C ABI (concept):
//   // Callback functions (C -> JS)
//   // It shall be called somewhere in the parse_bytes() function to report a packet.
//   extern void report_packet(const char *text, uint32_t len);
//   extern void report_packet_hex(const uint8_t *data, uint32_t len);
//
//   // Exported functions (JS -> C)
//   // Called by JS when: (1) this parser is loaded or right after compile,
//   // or (2) user clears receive/parsed data (Clear button). Use to reset internal buffer/state.
//   void reset_parser(void);  
//   // Called by JS whenever new bytes arrive.
//   void parse_bytes(const uint8_t *data, uint32_t len);  
//
// C keeps its own internal buffer/state and calls the callbacks
// whenever a full packet is parsed.

#include <stdint.h>
#include <stddef.h>

extern void report_packet(const char *text, uint32_t len);
extern void report_packet_hex(const uint8_t *data, uint32_t len);

// Simple line buffer
static uint8_t g_buf[4096];
static uint32_t g_len = 0;

void reset_parser(void) {
  g_len = 0;
}

void parse_bytes(const uint8_t *data, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    if (g_len >= sizeof(g_buf)) {
      // overflow: drop oldest data (very simple policy)
      g_len = 0;
    }
    g_buf[g_len++] = data[i];

    // check for CRLF
    if (g_len >= 2 && g_buf[g_len - 2] == '\r' && g_buf[g_len - 1] == '\n') {
      // one line: g_buf[0..g_len-2)
      uint32_t lineLen = g_len - 2;

      // text format: "line:<content>"
      // (for simplicity we just send the raw line as is)
      report_packet((const char *)g_buf, lineLen);

      // also send HEX of the full line
      report_packet_hex(g_buf, g_len);

      // reset buffer for next line
      g_len = 0;
    }
  }
}
