#ifndef SEQUENCE_PARTITION_GENERATOR_HPP_INCLUDED
#define SEQUENCE_PARTITION_GENERATOR_HPP_INCLUDED

#include <vector>
#include <stack>
#include <utility>
#include <memory>
#include <iostream>
#include <stdexcept>

#include "input-sequence.hpp"
#include "sequence-partition.hpp"

namespace echelon { namespace parsing { namespace impl { namespace unger { namespace internal {

template<typename T>
class SequencePartitionGenerator {
    std::stack<std::pair<int, std::shared_ptr<SequencePartition>>> generation_tracking_stack;
    unsigned number_of_sub_sequences;
    unsigned length_of_sequence;
public:
    SequencePartitionGenerator(echelon::parsing::api::InputSequence<T>* inputSequence, unsigned numberOfSubSequences) {
        if (numberOfSubSequences < 2) {
            throw new std::domain_error("Does not make sense to create fewer than two subsequences");
        }

        std::vector<unsigned> initialPartition(numberOfSubSequences);
        initialPartition.assign(numberOfSubSequences, 0);
        
        if (inputSequence == nullptr) {
            length_of_sequence = 0;
        }
        else {
            initialPartition[initialPartition.size() - 1] = inputSequence->length();
            length_of_sequence = inputSequence->length();
        }
        
        generation_tracking_stack.push(std::make_pair(
            numberOfSubSequences, 
            std::make_shared<SequencePartition>(SequencePartition(initialPartition))
        ));

        number_of_sub_sequences = numberOfSubSequences;
    }

    std::shared_ptr<SequencePartition> currentValue() {
        return generation_tracking_stack.top().second;
    }

    void moveNext() {
        auto tracking_info = generation_tracking_stack.top();

        // Post termination, in case moveNext is called again.
        if (tracking_info.second == nullptr) {
            return;
        }
        
        auto sequence_partition = tracking_info.second->getPartitionIndices();

        // Termination condition.
        if (sequence_partition[0] == length_of_sequence) {
            generation_tracking_stack.push(std::make_pair(0, nullptr));
            return;
        }

        auto take_from = tracking_info.first - 1;
        if (take_from > 0) {
            sequence_partition[take_from]--;
            sequence_partition[take_from - 1]++;

            generation_tracking_stack.push(std::make_pair(
                take_from,
                std::make_shared<SequencePartition>(SequencePartition(sequence_partition))
            ));
        }
        else {
            for (unsigned i = 1; i < number_of_sub_sequences; i++) {
                if (sequence_partition[i] != 0) {
                    auto size_of_target_sub_sequence = sequence_partition[i];

                    while (!generation_tracking_stack.empty()) {
                        auto potential_rewind_target = generation_tracking_stack.top();
                        generation_tracking_stack.pop();

                        if (generation_tracking_stack.top().second->getPartitionIndices()[i] != size_of_target_sub_sequence) {
                            generation_tracking_stack.push(potential_rewind_target);
                            break;
                        }
                    }

                    break;
                }
            }

            generation_tracking_stack.top().first++;
            moveNext();
        }
    }

private:
    void printStack() {
        std::cout << "\n";
        auto stack_copy = generation_tracking_stack;

        while (stack_copy.size() > 0) {
            auto tracking_info = stack_copy.top();
            stack_copy.pop();

            printTrackingInfo(tracking_info);
        }
        std::cout << "\n";
    }

    void printTrackingInfo(std::pair<int, std::shared_ptr<SequencePartition>> tracking_info) {
        std::cout << "Index " << tracking_info.first << ", [";
        auto indices = tracking_info.second->getPartitionIndices();
        for (unsigned i = 0; i < indices.size(); i++) {
            std::cout << indices[i];
            if (i + 1 != indices.size()) {
                std::cout << " | ";
            }
        }

        std::cout << "]\n";
    }
};

}}}}}

#endif // SEQUENCE_PARTITION_GENERATOR_HPP_INCLUDED
