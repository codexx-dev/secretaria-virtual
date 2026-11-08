import { type JSX, splitProps } from "solid-js";

import { twMerge } from "tailwind-merge";

import { useField } from "../forms/Field";
import { baseClass } from "./Input";

type SelectProps = JSX.SelectHTMLAttributes<HTMLSelectElement> & {
  //nada ;)
};

export function Select(props: SelectProps) {
  const [local, rest] = splitProps(props, ["class", "id"]);
  const field = useField();

  return (
    <select
      {...rest}
      id={local.id ?? field?.id}
      class={twMerge(baseClass, local.class ?? "")}
    />
  );
}
