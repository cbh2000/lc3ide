#ifndef SETTINGS_H
#define SETTINGS_H

// Not LC-3, in case of updates
#define LC3_WORD_SIZE (16) // In bits
#define LC3_WORD_TYPE quint16 // 16 bits
#define LC3_ADDRESS_SPACE ((qint32)(2 << LC3_WORD_SIZE)) // 2^16 = 65536
#define LC3_MEMORY_MAPPED_IO_START_ADDRESS (-2) // I don't know yet
#define LC3_REGISTER_COUNT (8) // From R0-R7

// Fixme: These should be implemented by the LC-3 simulator itself, not us
// We are the GUI.

#endif // SETTINGS_H
