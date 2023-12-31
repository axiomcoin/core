#ifndef NIRVANADELEGATION_H
#define NIRVANADELEGATION_H
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include <uint256.h>

class NirvanaDelegationPriv;
class ContractABI;

extern const std::string strDelegationsABI;
const ContractABI &DelegationABI();

struct Delegation
{
    Delegation():
        fee(0),
        blockHeight(0) {}

    bool IsNull() const
    {
        return staker == uint160() &&
                fee == 0 &&
                blockHeight == 0 &&
                PoD == std::vector<unsigned char>();
    }

    uint160 staker;
    uint8_t fee;
    uint32_t blockHeight;
    std::vector<unsigned char> PoD; //Proof Of Delegation
};

inline bool operator==(const Delegation& lhs, const Delegation& rhs)
{
    return lhs.staker == rhs.staker &&
           lhs.fee == rhs.fee &&
           lhs.blockHeight == rhs.blockHeight &&
           lhs.PoD == rhs.PoD;
}

inline bool operator!=(const Delegation& lhs, const Delegation& rhs)
{
    return !(lhs == rhs);
}

struct DelegationItem : public Delegation
{
    DelegationItem()
    {}

    bool IsNull() const
    {
        return Delegation::IsNull() &&
                delegate == uint160();
    }

    uint160 delegate;
};

enum DelegationType
{
    DELEGATION_NONE = 0,
    DELEGATION_ADD = 1,
    DELEGATION_REMOVE = 2,
};

struct DelegationEvent
{
    DelegationItem item;
    DelegationType type;

    DelegationEvent():
        type(DelegationType::DELEGATION_NONE)
    {}
};

/**
 * @brief The INirvanaStaker class Delegation filter
 */
class IDelegationFilter
{
public:
    virtual bool Match(const DelegationEvent& event) const = 0;
};

/**
 * @brief The NirvanaDelegation class Communicate with the nirvana delegation contract
 */
class NirvanaDelegation {
    
public:
    /**
     * @brief NirvanaDelegation Constructor
     */
    NirvanaDelegation();

    /**
     * @brief ~NirvanaDelegation Destructor
     */
    virtual ~NirvanaDelegation();

    /**
     * @brief GetDelegation Get delegation for an address
     * @param address Public key hash address
     * @param delegation Delegation information for an address
     * @return true/false
     */
    bool GetDelegation(const uint160& address, Delegation& delegation) const;

    /**
     * @brief VerifyDelegation Verify delegation for an address
     * @param address Public key hash address
     * @param delegation Delegation information for an address
     * @return true/false
     */
    static bool VerifyDelegation(const uint160& address, const Delegation& delegation);

    /**
     * @brief FilterDelegationEvents Filter delegation events
     * @param events Output list of delegation events for the filter
     * @param filter Delegation filter
     * @param fromBlock Start with block
     * @param toBlock End with block, -1 mean all available
     * @param minconf Minimum confirmations
     * @return true/false
     */
    bool FilterDelegationEvents(std::vector<DelegationEvent>& events, const IDelegationFilter& filter, int fromBlock = 0, int toBlock = -1, int minconf = 0) const;

    /**
     * @brief DelegationsFromEvents Get the delegations from the events
     * @param events Delegation event list
     * @return List of delegations
     */
    static std::map<uint160, Delegation> DelegationsFromEvents(const std::vector<DelegationEvent>& events);

    /**
     * @brief UpdateDelegationsFromEvents Update the delegations from the events
     * @param events Delegation event list
     * @param delegations List of delegations to be updated
     */
    static void UpdateDelegationsFromEvents(const std::vector<DelegationEvent>& events, std::map<uint160, Delegation>& delegations);

    /**
     * @brief ExistDelegationContract Delegation contract exist check
     * @return true/false
     */
    bool ExistDelegationContract() const;

    /**
     * @brief BytecodeRemove Bytecode for remove delegation
     * @return Bytecode
     */
    static std::string BytecodeRemove();

    /**
     * @brief BytecodeAdd Bytecode for add delegation
     * @param hexStaker Staker hex address
     * @param fee Staker fee
     * @param PoD Proof of delegation
     * @param datahex Bytecode
     * @param errorMessage Error message
     * @return true/false
     */
    static bool BytecodeAdd(const std::string& hexStaker, const int& fee, const std::vector<unsigned char>& PoD, std::string& datahex, std::string& errorMessage);

private:
    NirvanaDelegation(const NirvanaDelegation&);
    NirvanaDelegation& operator=(const NirvanaDelegation&);
    NirvanaDelegationPriv* priv;
};
#endif
