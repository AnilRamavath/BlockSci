//
//  address_writer.hpp
//  blocksci
//
//  Created by Harry Kalodner on 7/26/17.
//
//

#ifndef address_writer_hpp
#define address_writer_hpp

#include "parser_configuration.hpp"
#include "script_output.hpp"
#include "script_input.hpp"

#include <blocksci/file_mapper.hpp>
#include <blocksci/state.hpp>
#include <blocksci/address/address_info.hpp>
#include <blocksci/scripts/script_info.hpp>
#include <blocksci/scripts/script_data.hpp>

#include <boost/variant/variant.hpp>

template<typename T>
struct ScriptFileType;

template<typename T>
struct ScriptFileType<blocksci::FixedSize<T>> {
    using type = blocksci::FixedSizeFileMapper<T, boost::iostreams::mapped_file::readwrite>;
};

template<typename ...T>
struct ScriptFileType<blocksci::Indexed<T...>> {
    using type = blocksci::IndexedFileMapper<boost::iostreams::mapped_file::readwrite, T...>;
};


template<auto type>
using ScriptFileType_t = typename ScriptFileType<typename blocksci::ScriptInfo<type>::storage>::type;

template<auto type>
struct ScriptFile : public ScriptFileType_t<type> {
    using ScriptFileType_t<type>::ScriptFileType_t;
};

template<auto>
struct ScriptOutput;

template<auto>
struct ScriptInput;

class AddressWriter {
    using ScriptFilesTuple = blocksci::to_script_tuple_t<ScriptFile>;
    
    ScriptFilesTuple scriptFiles;
    
    template<blocksci::AddressType::Enum type>
    bool serializeImp(const ScriptInput<type> &input, const InputInfo &inputInfo);
    
    template<blocksci::AddressType::Enum type>
    void serializeImp(const ScriptOutput<type> &output);
    
public:
    
    template <blocksci::ScriptType::Enum type>
    ScriptFile<type> &getFile() {
        return std::get<ScriptFile<type>>(scriptFiles);
    }
    
    template <blocksci::ScriptType::Enum type>
    const ScriptFile<type> &getFile() const {
        return std::get<ScriptFile<type>>(scriptFiles);
    }
    
    template<blocksci::AddressType::Enum type>
    void serialize(const ScriptOutput<type> &output) {
        serializeImp(output);
    }
    
    template<blocksci::AddressType::Enum type>
    bool serialize(const ScriptInput<type> &input, const InputInfo &inputInfo) {
        return serializeImp(input, inputInfo);
    }
    
    void rollback(const blocksci::State &state);
    
    void serialize(const ScriptOutputType &output);
    void serialize(const ScriptInputType &input, const InputInfo &inputInfo);
    
    AddressWriter(const ParserConfigurationBase &config);
};

#endif /* address_writer_hpp */
