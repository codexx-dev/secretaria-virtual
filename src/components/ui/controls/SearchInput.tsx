import { type JSX, splitProps } from "solid-js";

import { twMerge } from "tailwind-merge";

import SearchIcon from "~/assets/search.svg";

import { useField } from "../forms/Field";
import { baseClass } from "./Input";

type SearchInputProps = JSX.InputHTMLAttributes<HTMLInputElement> & {
  //nada ;)
};

export function SearchInput(props: SearchInputProps) {
  const [local, rest] = splitProps(props, ["class", "id"]);
  const field = useField();

  return (
    <div class="relative">
      <input
        type="text"
        {...rest}
        id={local.id ?? field?.id}
        class={twMerge(baseClass, local.class ?? "")}
      />
      <button class="flex justify-around absolute w-14 right-0 top-1/2 -translate-y-1/2 opacity-50 cursor-pointer text-gray-900">
        {SearchIcon}
      </button>
    </div>
  );
}
