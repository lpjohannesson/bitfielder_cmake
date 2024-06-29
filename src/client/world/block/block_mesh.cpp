#include "block_mesh.h"

using namespace bf;

int BlockMesh::getSectionIndex(int blockY) {
    return blockY / SECTION_SIZE.y;
}