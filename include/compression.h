#ifndef COMPRESSION_H
#define COMPRESSION_H

int compressFile(const char *filePath);
int decompressFile(const char *filePath);
int compressFolder(const char *folderPath, const char *zipFilePath);
int decompressZip(const char *zipFilePath, const char *extractPath);

#endif
