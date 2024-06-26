#ifndef __StepOptionsGenerator__
#define __StepOptionsGenerator__

#include <vector>
#include <string>

namespace octopus
{
class Steppable;
class State;

/// @brief this simple class represents an steppable generator option
/// An option is a list of steps to be applied if the option
/// is popped from the option map for a player
class StepOptionsGenerator
{
public:
    StepOptionsGenerator(std::string const &key_p) : _key(key_p) {}
    virtual ~StepOptionsGenerator() {}

    /// @brief Create a new copy of this generator
    /// this is required to ease up memory handling between steps and states for options handling
    /// @return a newly created copy of this genertor
    virtual StepOptionsGenerator* newCopy() const = 0;

	/// @brief Generate the options based on the current state
	/// @param state_p the state to be used to generate the options
	virtual void genOptions(State const &state_p) = 0;

    /// @brief generate internal steppable for the given option
    /// @param options_p the option index
    /// @return a vector of steppables (owner is given away)
    virtual std::vector<Steppable *> genSteppables(State const &state_p, unsigned long options_p) const = 0;
    /// @brief Return the number of available options for this generator
    virtual unsigned long getNumOptions() const = 0;

	std::string const _key;
};

} // namespace octopus


#endif
