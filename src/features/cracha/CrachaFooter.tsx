import { appStyle } from "~/app";
import Button from "~/components/ui/Button";

export function CrachaFooter() {
  return (
    <div
      class={`flex gap-3 border-t p-4 ${appStyle.bgCol1} ${appStyle.textCol1} ${appStyle.borderCol1}`}
    >
      <Button class="ml-auto w-38">Excluir</Button>
      <Button class="w-38">Novo</Button>
    </div>
  );
}
