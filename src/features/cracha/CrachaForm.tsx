import { Show, createSignal } from "solid-js";
import { createStore } from "solid-js/store";

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
  const [values, setValues] = createStore<CrachaFormValues>({
    nome: props.initialValues?.nome ?? "",
    cargo: props.initialValues?.cargo ?? "",
    setor: props.initialValues?.setor ?? "",
    matricula: props.initialValues?.matricula ?? "",
    foto: props.initialValues?.foto ?? null,
  });
  const [previewUrl, setPreviewUrl] = createSignal<string | null>(null);

  const handleFoto = (event: Event & { currentTarget: HTMLInputElement }) => {
    const file = event.currentTarget.files?.[0] ?? null;
    setValues("foto", file);
  };

  const handleSubmit = async (event: SubmitEvent) => {
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
          onInput={(e) => setValues("nome", e.currentTarget.value)}
        />
      </Field>

      <div class="grid grid-cols-1 gap-5 sm:grid-cols-2">
        <Field label="Sala">
          <Input
            placeholder="1º ANO"
            value={values.cargo}
            onInput={(e) => setValues("cargo", e.currentTarget.value)}
          />
        </Field>

        <Field label="Ano">
          <Input
            placeholder="2026"
            value={values.setor}
            onInput={(e) => setValues("setor", e.currentTarget.value)}
          />
        </Field>
      </div>

      <Field label="Telefone">
        <Input
          inputmode="numeric"
          placeholder="000123"
          value={values.matricula}
          onInput={(e) => setValues("matricula", e.currentTarget.value)}
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
