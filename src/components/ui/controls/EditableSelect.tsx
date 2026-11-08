import { For, JSX, Show, createSignal, splitProps } from "solid-js";

import { twMerge } from "tailwind-merge";

import AddIcon from "~/assets/add.svg";
import EditIcon from "~/assets/edit.svg";

import Button from "../Button";
import { useField } from "../forms/Field";
import { baseClass, Input } from "./Input";

type EditableSelectProps = JSX.SelectHTMLAttributes<HTMLSelectElement> & {
  options?: Array<string>;
};

export function EditableSelect(props: EditableSelectProps) {
  const [options, setOptions] = createSignal([...(props.options ?? "")]);
  const [isOpen, setIsOpen] = createSignal(false);
  const field = useField();

  return (
    <div
      class={twMerge("relative w-full text-sm", props.class)}
      onFocusOut={(e) => {
        if (!e.currentTarget.contains(e.relatedTarget as Node | null)) {
          setIsOpen(false);
        }
      }}
    >
      <button
        id={props.id ?? field?.id}
        class={twMerge(baseClass, "flex justify-between")}
        disabled={props.disabled}
        aria-expanded={isOpen()}
        onClick={() => setIsOpen(!isOpen())}
      >
        <span class="text-gray-400">{"Selecione..."}</span>
        <span>▾</span>
      </button>

      <Show when={isOpen()}>
        <div class="absolute left-0 right-0 top-full z-50 mt-1 rounded-md border border-gray-200 bg-white p-1 shadow-lg">
          <For each={options()}>
            {(option, index) => (
              <div class="flex justify-between items-center px-2 w-full hover:bg-gray-100">
                <span>{"Selecione..."}</span>

                <Button variant="secondary" size="icon" class="bg-transparent">
                  {EditIcon}
                </Button>
              </div>
            )}
          </For>

          <div class="mt-1 flex items-center gap-1 border-t border-gray-200 pt-2">
            <Input placeholder="Nova opção..."/>

            <Button variant="primary" size="icon">
              {AddIcon}
            </Button>
          </div>
        </div>
      </Show>
    </div>
  );
}
