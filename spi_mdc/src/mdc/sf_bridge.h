#ifndef SF_BRIDGE_H
#define SF_BRIDGE_H

#ifdef __cplusplus
 extern "C" {
#endif
 int SF_bridge_init( void );
 int SF_bridge_sector_erase( uint32_t EraseStartAddress );
 int SF_bridge_read( uint32_t adr, uint32_t sz, uint8_t *buf );
 int SF_bridge_write( uint32_t adr, uint32_t sz, uint8_t *buf  );
 int SF_bridge_mass_erase( void );
 int SF_bridge_total_erase( void );
 int SF_bridge_id( uint8_t *mfc_id, uint16_t *dev_id );

 int SF_XmFlashDownload( int verbose );                 // Xmodem driven flash updater.
 int SF_XmFlashUpload( int verbose );
 int  getDataSize(void);

 #ifdef __cplusplus
}
#endif

 #endif // SF_BRIDGE_H
