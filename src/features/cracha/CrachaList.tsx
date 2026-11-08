import Button from "~/components/ui/Button";
import { SelectableList } from "~/components/ui/SelectableList";

type CrachaListProps = {
  crachasInfo: CrachaInfo[];
  class?: string;
  onSelectionChange?: (ids: string[]) => void;
};

export type CrachaInfo = {
  id: string;
  nome: string;
  sala: string;
};

const columns = {
  sala: "Sala",
  nome: "Nome",
};

export function CrachaList(props: CrachaListProps) {
  return (
    <SelectableList
      class={props.class}
      items={props.crachasInfo}
      columns={columns}
      actions={() => (
        <Button class="px-4" size="sm" onclick={() => console.log("ok")}>
          Editar
        </Button>
      )}
    />
  );
}
