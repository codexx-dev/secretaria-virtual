import { createSignal, For, JSX, Show, splitProps } from "solid-js";
import Button from "../Button";
import { twMerge } from "tailwind-merge";
import EditIcon from "~/assets/edit.svg";
import AddIcon  from "~/assets/add.svg";
import { useField } from "~/components/forms/Field";

type EditableSelectProps = JSX.SelectHTMLAttributes<HTMLSelectElement> & {
  options?: Array<string>
}

const baseClass = 
  "w-full rounded-lg border border-gray-300 bg-white px-3 py-2 text-sm text-gray-900 " +
  "placeholder:text-gray-400 focus:border-blue-500 focus:outline-none focus:ring-2 " +
  "focus:ring-blue-500/30 disabled:cursor-not-allowed disabled:bg-gray-100 ";

export function EditableSelect (props: EditableSelectProps){
  const [local, rest] = splitProps(props, ["class", "id"]);
  const [options, setOptions] = createSignal([...props.options ?? '']);
  const [isOpen, setIsOpen] = createSignal(false);
  const field = useField();

  return (
    <div class={twMerge("relative w-full text-sm", local.class)}
      onFocusOut={(e) => {
        if (!e.currentTarget.contains(e.relatedTarget as Node | null)) {
          setIsOpen(false)
        }
      }}
    >
      <button
        id={local.id ?? field?.id}
        class={baseClass + "flex justify-between"}
        disabled={props.disabled}
        aria-expanded={isOpen()}
        onClick={() => setIsOpen(!isOpen())}
      >
        <span class="text-gray-400">
          { "Selecione..."}
        </span>
        <span>▾</span>
      </button>

      <Show when={isOpen()}>
        <div class="absolute left-0 right-0 top-full z-50 mt-1 rounded-md border border-gray-200 bg-white p-1 shadow-lg">

          <For each={options()}>
            {(option, index) => (
              <div class="flex justify-between items-center px-2 py-1 w-full hover:bg-gray-100">
                
                <span>
                  { "Selecione..."}
                </span>

                <Button variant="secondary" class="w-8 h-8 p-0 bg-transparent">
                  {EditIcon}
                </Button>
              </div>
            )}
          </For>

          <div class="mt-1 flex items-center gap-1 border-t border-gray-200 pt-2">
            <input
              class="min-w-0 flex-1 rounded border border-gray-300 px-2 py-2 text-sm outline-none focus:border-blue-400 focus:ring-2 focus:ring-blue-100"
              placeholder="Nova opção..."
            />

            <Button variant="primary" class="p-0 h-9 w-9">
              {AddIcon}
            </Button>
          </div>

        </div>
      </Show>
    </div>
  );

}