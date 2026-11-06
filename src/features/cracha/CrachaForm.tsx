import { createSignal, Show } from "solid-js";
import { createStore } from "solid-js/store";
import Button from "~/components/Button";
import { EditableSelect } from "~/components/forms/EditableSelect";
import { FileInput } from "~/components/forms/FileInput";
import { TextInput } from "~/components/forms/TextInput";

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
    <form onSubmit={handleSubmit} class="mx-auto flex w-full max-w-lg flex-col gap-5" novalidate>
      
      <div class="flex flex-col gap-1.5">
        <label for="ume" class="text-sm font-medium text-gray-700">
          Escola
        </label>
        <EditableSelect
          id="ume"
          options={["test1", "test2"]}
        />
      </div>
      
      <div class="flex flex-col gap-1.5">
        <label for="nome" class="text-sm font-medium text-gray-700">
          Nome
        </label>
        <TextInput
          id="nome"
          placeholder="Maria da Silva"
          value={values.nome}
          onInput={(e) => setValues("nome", e.currentTarget.value)}
        />
      </div>
 
      <div class="grid grid-cols-1 gap-5 sm:grid-cols-2">
        <div class="flex flex-col gap-1.5">
          <label for="sala" class="text-sm font-medium text-gray-700">
            Sala
          </label>
          <TextInput
            id="sala"
            placeholder="1º ANO"
            value={values.cargo}
            onInput={(e) => setValues("cargo", e.currentTarget.value)}
          />
        </div>
 
        <div class="flex flex-col gap-1.5">
          <label for="ano" class="text-sm font-medium text-gray-700">
            Ano
          </label>
          <TextInput
            id="ano"
            placeholder="2026"
            value={values.setor}
            onInput={(e) => setValues("setor", e.currentTarget.value)}
          />
        </div>
      </div>
 
      <div class="flex flex-col gap-1.5">
        <label for="telefone" class="text-sm font-medium text-gray-700">
          Telefone
        </label>
        <TextInput
          id="telefone"
          inputmode="numeric"
          placeholder="000123"
          value={values.matricula}
          onInput={(e) => setValues("matricula", e.currentTarget.value)}
        />
      </div>

      <div class="flex flex-col gap-1.5">
        <label for="responsavel" class="text-sm font-medium text-gray-700">
          Responsável
        </label>
        <TextInput
          id="responsavel"
          placeholder="Maria da Silva"
        />
      </div>

      <div class="flex flex-col gap-1.5">
        <label for="autorizados" class="text-sm font-medium text-gray-700">
          Autorizados
        </label>
        <TextInput
          id="autorizados"
          placeholder="José; Débora; Gabriel"
        />
      </div>
 

      <div class="flex items-center gap-4">
        <div class="flex h-40 w-30 shrink-0 items-center justify-center overflow-hidden rounded-md border border-dashed border-gray-300 bg-gray-50">
          <Show
            when={previewUrl()}
            fallback={<span class="text-xs text-gray-400">Sem foto</span>}
          >
            {(url) => <img src={url()} alt="Pré-visualização da foto" class="h-full w-full object-cover" />}
          </Show>
        </div>
        <FileInput
          id="foto"
          accept="image/*"
          onChange={handleFoto}
        />
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
};