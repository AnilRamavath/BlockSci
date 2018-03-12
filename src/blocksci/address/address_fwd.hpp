//
//  address_fwd.hpp
//  blocksci
//
//  Created by Harry Kalodner on 11/8/17.
//

#ifndef address_fwd_h
#define address_fwd_h

#include "address_types.hpp"
#include "equiv_address_type.hpp"

namespace blocksci {
    struct RawAddress;
    
    class Address;
    class EquivAddress;
    
    template <AddressType::Enum>
    struct AddressInfo;
    
    template <EquivAddressType::Enum>
    struct EquivAddressInfo;
}

#endif /* address_fwd_h */
