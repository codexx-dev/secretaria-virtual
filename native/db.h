#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

// Par coluna/valor de um registro. Os dados chegam em tempo de execucao
// (do JSON da requisicao), por isso vector e nao initializer_list.
using Field  = std::pair<std::string, std::string>;
using Record = std::vector<Field>;

class DB {
private:

public:
  bool create(
    std::string_view name,
    const std::vector<std::string>& columns,
    const std::vector<std::string>& keys
  );
  bool remove(std::string_view name);

  bool insert_val(std::string_view name, const Record& record);
  bool update_val(
    std::string_view name,
    std::string_view column,
    std::string_view value
  );
  bool delete_val(std::string_view name);
};
