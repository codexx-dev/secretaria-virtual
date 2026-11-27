import { Show, createContext, createUniqueId, useContext } from "solid-js";

import type { JSX } from "@solidjs/web";

import { twMerge } from "tailwind-merge";

type FieldProps = Omit<JSX.HTMLAttributes<HTMLDivElement>, "class"> & {
  class?: string;
  label: string;
  errors?: string;
};
type FieldContextValue = {
  id: string;
};

// null = controle usado fora de um <Field>.
const FieldContext = createContext<FieldContextValue | null>(null);

export const useField = () => useContext(FieldContext);

export function Field(props: FieldProps) {
  const id = createUniqueId();

  return (
    <FieldContext value={{ id }}>
      <div class={twMerge("flex flex-col gap-1.5", props.class)}>
        <label for={id} class="text-sm font-medium">
          {props.label}
        </label>

        {props.children}

        <Show when={props.errors}>
          <p class="text-xs text-red-700">{props.errors}</p>
        </Show>
      </div>
    </FieldContext>
  );
}
