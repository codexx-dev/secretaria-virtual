import { JSX, splitProps } from "solid-js";

import { twMerge } from "tailwind-merge";

import { useField } from "../forms/Field";

type FileInputProps = JSX.InputHTMLAttributes<HTMLInputElement> & {
  //nada ;)
};

export const baseClass =
  "block w-full text-sm text-gray-600 file:mr-3 file:rounded-lg file:border-0 " +
  "file:bg-blue-50 file:px-3 file:py-2 file:text-sm file:font-medium " +
  "file:text-blue-700 hover:file:bg-blue-100 disabled:cursor-not-allowed";

export function FileInput(props: FileInputProps) {
  const [local, rest] = splitProps(props, ["class", "id"]);
  const field = useField();

  return (
    <input
      type="file"
      {...rest}
      id={local.id ?? field?.id}
      class={twMerge(baseClass, local.class ?? "")}
    />
  );
}
