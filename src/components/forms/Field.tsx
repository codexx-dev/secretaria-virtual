import { createContext, createUniqueId, JSX, Show, useContext } from "solid-js";

type FieldProps = {
  label: string;
  errors?: string;
  children: JSX.Element;
}
type FieldContextValue = {
  id: string;
}

const FieldContext = createContext<FieldContextValue>();

export const useField = () => useContext(FieldContext);

export function Field(props: FieldProps){
    const id = createUniqueId();

  return(
    <FieldContext.Provider value={{ id }}>
      <div class="flex flex-col gap-1.5">
        <label for={id} class="text-sm font-medium text-gray-700">
          {props.label}
        </label>

        {props.children}

        <Show when={props.errors}>
          <p class="text-xs text-red-600">{props.errors}</p>
        </Show>
      </div>
    </FieldContext.Provider>
  );
}