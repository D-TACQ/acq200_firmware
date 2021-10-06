/*
 * ebsa_app_layer.h
 */


extern "C" 
 void* ebsa_get_base( const char* fname, const char* prot );
/* returns mapping of base address for device
 *
 * inputs: fname - the device name
 *         prot  - r - readable, w - writeable 
 */
