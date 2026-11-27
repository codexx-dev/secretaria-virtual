import { omit } from "solid-js";

import type { JSX } from "@solidjs/web";

import { twMerge } from "tailwind-merge";

import SearchIcon from "~/assets/search.svg";

import { useField } from "../forms/Field";
import { baseClass } from "./Input";

type SearchInputProps = Omit<
  JSX.InputHTMLAttributes<HTMLInputElement>,
  "class"
> & {
  class?: string;
};

export function SearchInput(props: SearchInputProps) {
  const rest = omit(props, "class", "id");
  const field = useField();

  return (
    <div class="relative">
      <input
        type="text"
        {...rest}
        id={props.id ?? field?.id}
        class={twMerge(baseClass, props.class ?? "")}
      />
      <button class="flex justify-around absolute w-14 right-0 top-1/2 -translate-y-1/2 opacity-50 cursor-pointer text-gray-900">
        <SearchIcon />
      </button>
    </div>
  );
}
