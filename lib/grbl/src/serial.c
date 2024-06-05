#include "grbl.h"

int tty_prev_func(int ch) {
  switch (ch) {
    case CMD_RESET:         mc_reset(); break; // Call motion control reset routine.
    case CMD_STATUS_REPORT: system_set_exec_state_flag(EXEC_STATUS_REPORT); break; // Set as true
    case CMD_CYCLE_START:   system_set_exec_state_flag(EXEC_CYCLE_START); break; // Set as true
    case CMD_FEED_HOLD:     system_set_exec_state_flag(EXEC_FEED_HOLD); break; // Set as true
    default :
      if( ch < 0x80 ) return ch;
      // Real-time control characters are extended ACSII only.
      switch(ch) {
        case CMD_SAFETY_DOOR:   system_set_exec_state_flag(EXEC_SAFETY_DOOR); break; // Set as true
        case CMD_JOG_CANCEL:   
          if (sys.state & STATE_JOG) { // Block all other states from invoking motion cancel.
            system_set_exec_state_flag(EXEC_MOTION_CANCEL); 
          }
          break; 
        #ifdef DEBUG
          case CMD_DEBUG_REPORT: {uint8_t sreg = SREG; cli(); bit_true(sys_rt_exec_debug,EXEC_DEBUG_REPORT); SREG = sreg;} break;
        #endif
        case CMD_FEED_OVR_RESET: system_set_exec_motion_override_flag(EXEC_FEED_OVR_RESET); break;
        case CMD_FEED_OVR_COARSE_PLUS: system_set_exec_motion_override_flag(EXEC_FEED_OVR_COARSE_PLUS); break;
        case CMD_FEED_OVR_COARSE_MINUS: system_set_exec_motion_override_flag(EXEC_FEED_OVR_COARSE_MINUS); break;
        case CMD_FEED_OVR_FINE_PLUS: system_set_exec_motion_override_flag(EXEC_FEED_OVR_FINE_PLUS); break;
        case CMD_FEED_OVR_FINE_MINUS: system_set_exec_motion_override_flag(EXEC_FEED_OVR_FINE_MINUS); break;
        case CMD_RAPID_OVR_RESET: system_set_exec_motion_override_flag(EXEC_RAPID_OVR_RESET); break;
        case CMD_RAPID_OVR_MEDIUM: system_set_exec_motion_override_flag(EXEC_RAPID_OVR_MEDIUM); break;
        case CMD_RAPID_OVR_LOW: system_set_exec_motion_override_flag(EXEC_RAPID_OVR_LOW); break;
        case CMD_SPINDLE_OVR_RESET: system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_RESET); break;
        case CMD_SPINDLE_OVR_COARSE_PLUS: system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_COARSE_PLUS); break;
        case CMD_SPINDLE_OVR_COARSE_MINUS: system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_COARSE_MINUS); break;
        case CMD_SPINDLE_OVR_FINE_PLUS: system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_FINE_PLUS); break;
        case CMD_SPINDLE_OVR_FINE_MINUS: system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_FINE_MINUS); break;
        case CMD_SPINDLE_OVR_STOP: system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_STOP); break;
        case CMD_COOLANT_FLOOD_OVR_TOGGLE: system_set_exec_accessory_override_flag(EXEC_COOLANT_FLOOD_OVR_TOGGLE); break;
        #ifdef ENABLE_M7
          case CMD_COOLANT_MIST_OVR_TOGGLE: system_set_exec_accessory_override_flag(EXEC_COOLANT_MIST_OVR_TOGGLE); break;
        #endif
        default:
          return ch;
    }
  }
  return 0; // This ch has already been handled, so can throw it away.
}
