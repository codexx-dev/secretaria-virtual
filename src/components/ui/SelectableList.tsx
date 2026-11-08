import { For, JSX, createMemo, createSignal } from "solid-js";

import { twMerge } from "tailwind-merge";

type SelectableListColumns<T> = { [K in keyof T]?: string };

type SelectableListProps<T extends { id: string }> = {
  items: T[];
  columns: SelectableListColumns<T>;
  class?: string;
  actions?: () => JSX.Element;
};

export function SelectableList<T extends { id: string }>(
  props: SelectableListProps<T>,
) {
  const [selected, setSelected] = createSignal(new Set<string>());

  const all = () =>
    props.items.length > 0 && selected().size === props.items.length;

  const toggle = (id: string) => {
    const next = new Set(selected());
    if (next.has(id)) next.delete(id);
    else next.add(id);
    setSelected(next);
  };

  const toggleAll = () => {
    setSelected(
      all() ? new Set<string>() : new Set(props.items.map((i) => i.id)),
    );
  };

  const keys = () => Object.keys(props.columns) as (keyof T & string)[];

  const widths = createMemo(() =>
    keys().map((key, i, arr) => {
      if (i === arr.length - 1) return undefined;
      const label = props.columns[key] ?? "";
      const labelSize = Math.ceil(label.length * 1.3);
      const max = Math.max(
        labelSize,
        ...props.items.map((item) => String(item[key]).length),
      );
      return `${max}ch`;
    }),
  );

  const cellClass = (i: number) =>
    widths()[i] ? "shrink-0 whitespace-nowrap" : "min-w-0 grow truncate";

  return (
    <ul
      class={twMerge(
        "divide-y divide-gray-200 overflow-y-auto rounded-lg border border-gray-200 bg-white",
        props.class,
      )}
    >
      <li class="sticky top-0 z-10 shadow-[0_1px_0_#d1d5db]">
        <label class="flex items-center gap-3 bg-gray-200 px-3 py-2 text-sm font-semibold uppercase tracking-wide text-gray-700">
          <input
            type="checkbox"
            class="h-4 w-4 accent-blue-600"
            aria-label="Selecionar todos"
            checked={all()}
            onChange={toggleAll}
          />
          <For each={keys()}>
            {(key, i) => (
              <span
                class={twMerge(
                  cellClass(i()),
                  i() > 0 && "border-l border-gray-400 pl-3",
                )}
                style={{ width: widths()[i()] }}
              >
                {props.columns[key]}
              </span>
            )}
          </For>
        </label>
      </li>

      <For each={props.items}>
        {(item) => (
          <li>
            <label
              class={twMerge(
                "group flex cursor-pointer items-center gap-3 px-3 py-1 text-sm text-gray-900 hover:bg-blue-100",
                selected().has(item.id) && "bg-blue-50",
              )}
            >
              <input
                type="checkbox"
                class="h-4 w-4 accent-blue-600"
                checked={selected().has(item.id)}
                onChange={() => toggle(item.id)}
              />
              <For each={keys()}>
                {(key, i) => (
                  <span
                    class={twMerge(cellClass(i()), i() > 0 && "pl-3")}
                    style={{ width: widths()[i()] }}
                  >
                    {String(item[key])}
                  </span>
                )}
              </For>

              <div class="invisible group-hover:visible">
                {props.actions?.()}
              </div>
            </label>
          </li>
        )}
      </For>
    </ul>
  );
}
