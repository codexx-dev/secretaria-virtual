import { omit } from "solid-js";

import type { JSX } from "@solidjs/web";

import { twMerge } from "tailwind-merge";

import { useField } from "../forms/Field";

type InputProps = Omit<JSX.InputHTMLAttributes<HTMLInputElement>, "class"> & {
  class?: string;
};

export const baseClass =
  "w-full rounded-lg border border-gray-300 bg-white px-3 py-2 text-sm text-gray-900 " +
  "placeholder:text-gray-400 focus:border-blue-500 focus:outline-none focus:ring-2 " +
  "focus:ring-blue-500/30 disabled:cursor-not-allowed disabled:bg-gray-100";

export function Input(props: InputProps) {
  const rest = omit(props, "class", "id");
  const field = useField();

  return (
    <input
      type="text"
      {...rest}
      id={props.id ?? field?.id}
      class={twMerge(baseClass, props.class ?? "")}
    />
  );
}
