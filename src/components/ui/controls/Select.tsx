import { omit } from "solid-js";

import type { JSX } from "@solidjs/web";

import { twMerge } from "tailwind-merge";

import { useField } from "../forms/Field";
import { baseClass } from "./Input";

type SelectProps = Omit<
  JSX.SelectHTMLAttributes<HTMLSelectElement>,
  "class"
> & {
  class?: string;
};

export function Select(props: SelectProps) {
  const rest = omit(props, "class", "id");
  const field = useField();

  return (
    <select
      {...rest}
      id={props.id ?? field?.id}
      class={twMerge(baseClass, props.class ?? "")}
    />
  );
}
