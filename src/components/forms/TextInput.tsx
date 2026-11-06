
import { splitProps, type JSX } from "solid-js";
import { twMerge } from "tailwind-merge";

type TextInputProps = JSX.InputHTMLAttributes<HTMLInputElement> & {
  //nada ;)
}

const baseClass =
  "w-full rounded-lg border border-gray-300 bg-white px-3 py-2 text-sm text-gray-900 " +
  "placeholder:text-gray-400 focus:border-blue-500 focus:outline-none focus:ring-2 " +
  "focus:ring-blue-500/30 disabled:cursor-not-allowed disabled:bg-gray-100";

export function TextInput(props: TextInputProps) {
  const [local, rest] = splitProps(props, ["class"]);

  return (
    <input
      type="text"
      class={twMerge(baseClass, local.class ?? '')}
      {...rest}
    />
  );
}