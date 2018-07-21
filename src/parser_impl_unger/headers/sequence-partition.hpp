#ifndef SEQUENCE_PARTITION_HPP_INCLUDED
#define SEQUENCE_PARTITION_HPP_INCLUDED

#include <vector>

namespace echelon { namespace parsing { namespace impl { namespace unger { namespace internal {

class SequencePartition {
    std::vector<unsigned> partition_indices; 
public:
    SequencePartition(std::vector<unsigned> partitionIndices) {
        partition_indices = partitionIndices;
    }
};

}}}}}

#endif // SEQUENCE_PARTITION_HPP_INCLUDED
