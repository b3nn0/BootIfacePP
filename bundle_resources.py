#!/usr/bin/python3

from pathlib import Path
import os
import gzip


def varname(name):
    return 'bootiface' + '_' + name.replace('.', '_')

def bundleFile(path : Path):
    fname = path.name
    outpath = Path('src/data_' + fname + '.cpp')
    if outpath.exists():
        # only update if newer
        istat = os.stat(path.absolute())
        ostat = os.stat(outpath.absolute())
        if ostat.st_mtime >= istat.st_mtime:
            return # already up to date
    
    content = open(path.absolute(), 'rb').read()
    origLen = len(content)
    compressed = gzip.compress(content)


    with open(outpath.absolute(), 'wb') as f:
        f.write(b'#include<string>\n')
        f.write(b'#include<cstdint>\n')
        f.write(b'#ifndef PROGMEM\n')
        f.write(b'#define PROGMEM\n')
        f.write(b'#endif\n')
        var = varname(fname)
        f.write(bytes('const uint8_t {}_arr[] PROGMEM = {{'.format(var), 'utf-8'))
        first = True
        for b in compressed:
            if not first:
                f.write(b',')
            f.write(bytes(str(b), 'utf-8'))
            first = False
        f.write(b'};\n')
        f.write(bytes('extern const std::string_view {0}((const char*) {0}_arr, {1});\n'.format(var, len(compressed)), 'utf-8'))
        f.write(bytes('extern const size_t {0}_orig_len = {1};\n'.format(var, origLen), 'utf-8'))



def bundleDataDir():
    print('Bundle data files')
    for path in Path('data').iterdir():
        bundleFile(path)

#if __name__ == '__main__':
bundleDataDir()


