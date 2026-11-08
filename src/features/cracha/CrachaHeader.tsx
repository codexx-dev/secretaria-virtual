import { For } from "solid-js";

import { appStyle } from "~/app";
import { Input } from "~/components/ui/controls/Input";
import { SearchInput } from "~/components/ui/controls/SearchInput";
import { Select } from "~/components/ui/controls/Select";
import { Field } from "~/components/ui/forms/Field";

const salas = [
  "M II",
  "JD",
  "PRÉ",
  "1º ANO",
  "2º ANO",
  "3º ANO",
  "4º ANO",
  "5º ANO",
  "6º ANO",
  "7º ANO",
  "8º ANO",
  "9º ANO",
];

export function CrachaHeader() {
  const currentYear = () => new Date().getFullYear();

  return (
    <div
      class={`flex items-end gap-3 p-4 border-b [&_select,input]:py-1 ${appStyle.bgCol1} ${appStyle.textCol1} ${appStyle.borderCol1}`}
    >
      <Field label="Escola" class="min-w-0 flex-1">
        <Select>
          <option value="">Selecione a escola</option>
          <option value="">....</option>
        </Select>
      </Field>

      <Field label="Sala" class="shrink-0">
        <Select>
          <option value="">Selecionar</option>
          <For each={salas}>{(n) => <option value={n}>{n}</option>}</For>
          <option value="all">Todas</option>
        </Select>
      </Field>

      <Field label="Tipo de crachá" class="shrink-0">
        <Select>
          <option value="regular">Regular</option>
          <option value="ampliada">Jornada ampliada</option>
        </Select>
      </Field>

      <Field label="Ano" class="shrink-0">
        <Input type="number" inputMode="numeric" value={currentYear()} />
      </Field>

      <Field label="Pesquisar" class="min-w-0 flex-1">
        <SearchInput placeholder="Buscar por nome..." />
      </Field>
    </div>
  );
}
