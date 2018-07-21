#ifndef SEQUENCE_PARTITION_GENERATOR_HPP_INCLUDED
#define SEQUENCE_PARTITION_GENERATOR_HPP_INCLUDED

#include <vector>

#include "input-sequence.hpp"
#include "sequence-partition.hpp"

namespace echelon { namespace parsing { namespace impl { namespace unger { namespace internal {

template<typename T>
class SequencePartitionGenerator {
    std::shared_ptr<SequencePartition> current_sequence_partition;
public:
    SequencePartitionGenerator(InputSequence<T>* inputSequence, unsigned numberOfSubSequences) {
        if (numberOfSubSequences < 2) {
            throw new std::domain_error("Does not make sense to create fewer than two subsequences");
        }

        std::vector<unsigned> initialPartition(numberOfSubSequences - 1);
        initialPartition.assign(numberOfSubSequences - 1, 0);
        initialPartition[initialPartition.size() - 1] = inputSequence.length();
        
        current_sequence_partition = std::make_shared(SequencePartition(initialPartition));
    }

    std::shared_ptr<SequencePartition> currentValue() {
        return current_sequence_partition;
    }
};

}}}}}

#endif // SEQUENCE_PARTITION_GENERATOR_HPP_INCLUDED
