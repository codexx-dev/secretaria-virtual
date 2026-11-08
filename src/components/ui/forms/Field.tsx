import { JSX, Show, createContext, createUniqueId, useContext } from "solid-js";

import { twMerge } from "tailwind-merge";

type FieldProps = JSX.HTMLAttributes<HTMLDivElement> & {
  label: string;
  errors?: string;
};
type FieldContextValue = {
  id: string;
};

const FieldContext = createContext<FieldContextValue>();

export const useField = () => useContext(FieldContext);

export function Field(props: FieldProps) {
  const id = createUniqueId();

  return (
    <FieldContext.Provider value={{ id }}>
      <div class={twMerge("flex flex-col gap-1.5", props.class)}>
        <label for={id} class="text-sm font-medium">
          {props.label}
        </label>

        {props.children}

        <Show when={props.errors}>
          <p class="text-xs text-red-700">{props.errors}</p>
        </Show>
      </div>
    </FieldContext.Provider>
  );
}
