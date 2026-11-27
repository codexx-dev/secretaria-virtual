import { Show, createSignal, createStore } from "solid-js";

import type { JSX } from "@solidjs/web";

import Button from "~/components/ui/Button";
import { EditableSelect } from "~/components/ui/controls/EditableSelect";
import { FileInput } from "~/components/ui/controls/FileInput";
import { Input } from "~/components/ui/controls/Input";
import { Field } from "~/components/ui/forms/Field";

type CrachaFormValues = {
  nome: string;
  cargo: string;
  setor: string;
  matricula: string;
  foto: File | null;
};

type CrachaFormProps = {
  initialValues?: Partial<CrachaFormValues>;
  submitLabel?: string;
  disabled?: boolean;
  onSubmit: (values: CrachaFormValues) => void | Promise<void>;
};

export default function CrachaForm(props: CrachaFormProps) {
  const [values, setValues] = createStore<CrachaFormValues>(
    (draft) => {
      draft.nome = props.initialValues?.nome ?? "";
      draft.cargo = props.initialValues?.cargo ?? "";
      draft.setor = props.initialValues?.setor ?? "";
      draft.matricula = props.initialValues?.matricula ?? "";
      draft.foto = props.initialValues?.foto ?? null;
    },
    { nome: "", cargo: "", setor: "", matricula: "", foto: null },
  );

  const setText = (
    key: "nome" | "cargo" | "setor" | "matricula",
    value: string,
  ) =>
    setValues((draft) => {
      draft[key] = value;
    });
  const [previewUrl, setPreviewUrl] = createSignal<string | null>(null);

  const handleFoto = (event: Event & { currentTarget: HTMLInputElement }) => {
    const file = event.currentTarget.files?.[0] ?? null;
    setValues((draft) => {
      draft.foto = file;
    });
  };

  const handleSubmit: JSX.EventHandler<HTMLFormElement, SubmitEvent> = (
    event,
  ) => {
    event.preventDefault();
  };

  return (
    <form
      onSubmit={handleSubmit}
      class="mx-auto flex w-full max-w-lg flex-col gap-3"
      novalidate
    >
      <Field label="Escola">
        <EditableSelect options={["test1", "test2"]} />
      </Field>

      <Field label="Nome">
        <Input
          placeholder="Maria da Silva"
          value={values.nome}
          onInput={(e) => setText("nome", e.currentTarget.value)}
        />
      </Field>

      <div class="grid grid-cols-1 gap-5 sm:grid-cols-2">
        <Field label="Sala">
          <Input
            placeholder="1º ANO"
            value={values.cargo}
            onInput={(e) => setText("cargo", e.currentTarget.value)}
          />
        </Field>

        <Field label="Ano">
          <Input
            placeholder="2026"
            value={values.setor}
            onInput={(e) => setText("setor", e.currentTarget.value)}
          />
        </Field>
      </div>

      <Field label="Telefone">
        <Input
          inputmode="numeric"
          placeholder="000123"
          value={values.matricula}
          onInput={(e) => setText("matricula", e.currentTarget.value)}
        />
      </Field>

      <Field label="Responsável">
        <Input placeholder="Maria da Silva" />
      </Field>

      <Field label="Autorizados">
        <Input placeholder="José; Débora; Gabriel" />
      </Field>

      <div class="flex items-center gap-4">
        <div class="flex h-40 w-30 shrink-0 items-center justify-center overflow-hidden rounded-md border border-dashed border-gray-300 bg-gray-50">
          <Show
            when={previewUrl()}
            fallback={<span class="text-xs text-gray-400">Sem foto</span>}
          >
            {(url) => (
              <img
                src={url()}
                alt="Pré-visualização da foto"
                class="h-full w-full object-cover"
              />
            )}
          </Show>
        </div>
        <FileInput accept="image/*" onChange={handleFoto} />
      </div>

      <div class="flex flex-row gap-5">
        <Button variant="secondary" class="w-full">
          Cancelar
        </Button>

        <Button type="submit" variant="primary" class="w-full">
          Criar crachá
        </Button>
      </div>
    </form>
  );
}
