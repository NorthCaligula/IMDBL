PGDMP  4                    |         	   IMDBl_new    15.4    16.4 ?    F           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                      false            G           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                      false            H           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                      false            I           1262    119357 	   IMDBl_new    DATABASE        CREATE DATABASE "IMDBl_new" WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'Russian_Russia.1251';
    DROP DATABASE "IMDBl_new";
                postgres    false            �            1259    119359    content    TABLE     z   CREATE TABLE public.content (
    id_content integer NOT NULL,
    name text,
    description text,
    awards integer
);
    DROP TABLE public.content;
       public         heap    postgres    false            �            1259    119358    content_id_content_seq    SEQUENCE     �   CREATE SEQUENCE public.content_id_content_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 -   DROP SEQUENCE public.content_id_content_seq;
       public          postgres    false    215            J           0    0    content_id_content_seq    SEQUENCE OWNED BY     Q   ALTER SEQUENCE public.content_id_content_seq OWNED BY public.content.id_content;
          public          postgres    false    214            �            1259    119393    journal    TABLE     �   CREATE TABLE public.journal (
    id_writing integer NOT NULL,
    id_user integer NOT NULL,
    id_content integer NOT NULL,
    id_rating integer,
    id_review integer
);
    DROP TABLE public.journal;
       public         heap    postgres    false            �            1259    119392    journal_id_writing_seq    SEQUENCE     �   CREATE SEQUENCE public.journal_id_writing_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 -   DROP SEQUENCE public.journal_id_writing_seq;
       public          postgres    false    223            K           0    0    journal_id_writing_seq    SEQUENCE OWNED BY     Q   ALTER SEQUENCE public.journal_id_writing_seq OWNED BY public.journal.id_writing;
          public          postgres    false    222            �            1259    119400    member    TABLE     p   CREATE TABLE public.member (
    id_member integer NOT NULL,
    name_member text,
    awards_member integer
);
    DROP TABLE public.member;
       public         heap    postgres    false            �            1259    119399    member_id_member_seq    SEQUENCE     �   CREATE SEQUENCE public.member_id_member_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.member_id_member_seq;
       public          postgres    false    225            L           0    0    member_id_member_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.member_id_member_seq OWNED BY public.member.id_member;
          public          postgres    false    224            �            1259    119418    part_member    TABLE     �   CREATE TABLE public.part_member (
    id_part integer NOT NULL,
    id_member integer NOT NULL,
    id_type integer NOT NULL,
    id_content integer NOT NULL
);
    DROP TABLE public.part_member;
       public         heap    postgres    false            �            1259    119417    part_member_id_part_seq    SEQUENCE     �   CREATE SEQUENCE public.part_member_id_part_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.part_member_id_part_seq;
       public          postgres    false    229            M           0    0    part_member_id_part_seq    SEQUENCE OWNED BY     S   ALTER SEQUENCE public.part_member_id_part_seq OWNED BY public.part_member.id_part;
          public          postgres    false    228            �            1259    119377    rating    TABLE     �   CREATE TABLE public.rating (
    id_rating integer NOT NULL,
    id_user integer NOT NULL,
    id_content integer NOT NULL,
    rating integer NOT NULL
);
    DROP TABLE public.rating;
       public         heap    postgres    false            �            1259    119376    rating_id_rating_seq    SEQUENCE     �   CREATE SEQUENCE public.rating_id_rating_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.rating_id_rating_seq;
       public          postgres    false    219            N           0    0    rating_id_rating_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.rating_id_rating_seq OWNED BY public.rating.id_rating;
          public          postgres    false    218            �            1259    119384    review    TABLE     �   CREATE TABLE public.review (
    id_review integer NOT NULL,
    id_content integer NOT NULL,
    id_user integer NOT NULL,
    text_review text NOT NULL,
    recomendation boolean
);
    DROP TABLE public.review;
       public         heap    postgres    false            �            1259    119383    review_id_review_seq    SEQUENCE     �   CREATE SEQUENCE public.review_id_review_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.review_id_review_seq;
       public          postgres    false    221            O           0    0    review_id_review_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.review_id_review_seq OWNED BY public.review.id_review;
          public          postgres    false    220            �            1259    119409    type_member    TABLE     X   CREATE TABLE public.type_member (
    id_type integer NOT NULL,
    type_member text
);
    DROP TABLE public.type_member;
       public         heap    postgres    false            �            1259    119408    type_member_id_type_seq    SEQUENCE     �   CREATE SEQUENCE public.type_member_id_type_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.type_member_id_type_seq;
       public          postgres    false    227            P           0    0    type_member_id_type_seq    SEQUENCE OWNED BY     S   ALTER SEQUENCE public.type_member_id_type_seq OWNED BY public.type_member.id_type;
          public          postgres    false    226            �            1259    119368    user    TABLE     �   CREATE TABLE public."user" (
    id_user integer NOT NULL,
    login text NOT NULL,
    password text NOT NULL,
    email text NOT NULL,
    gender integer
);
    DROP TABLE public."user";
       public         heap    postgres    false            �            1259    119367    user_id_user_seq    SEQUENCE     �   CREATE SEQUENCE public.user_id_user_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 '   DROP SEQUENCE public.user_id_user_seq;
       public          postgres    false    217            Q           0    0    user_id_user_seq    SEQUENCE OWNED BY     G   ALTER SEQUENCE public.user_id_user_seq OWNED BY public."user".id_user;
          public          postgres    false    216            �           2604    119362    content id_content    DEFAULT     x   ALTER TABLE ONLY public.content ALTER COLUMN id_content SET DEFAULT nextval('public.content_id_content_seq'::regclass);
 A   ALTER TABLE public.content ALTER COLUMN id_content DROP DEFAULT;
       public          postgres    false    214    215    215            �           2604    119396    journal id_writing    DEFAULT     x   ALTER TABLE ONLY public.journal ALTER COLUMN id_writing SET DEFAULT nextval('public.journal_id_writing_seq'::regclass);
 A   ALTER TABLE public.journal ALTER COLUMN id_writing DROP DEFAULT;
       public          postgres    false    223    222    223            �           2604    119403    member id_member    DEFAULT     t   ALTER TABLE ONLY public.member ALTER COLUMN id_member SET DEFAULT nextval('public.member_id_member_seq'::regclass);
 ?   ALTER TABLE public.member ALTER COLUMN id_member DROP DEFAULT;
       public          postgres    false    225    224    225            �           2604    119421    part_member id_part    DEFAULT     z   ALTER TABLE ONLY public.part_member ALTER COLUMN id_part SET DEFAULT nextval('public.part_member_id_part_seq'::regclass);
 B   ALTER TABLE public.part_member ALTER COLUMN id_part DROP DEFAULT;
       public          postgres    false    229    228    229            �           2604    119380    rating id_rating    DEFAULT     t   ALTER TABLE ONLY public.rating ALTER COLUMN id_rating SET DEFAULT nextval('public.rating_id_rating_seq'::regclass);
 ?   ALTER TABLE public.rating ALTER COLUMN id_rating DROP DEFAULT;
       public          postgres    false    218    219    219            �           2604    119387    review id_review    DEFAULT     t   ALTER TABLE ONLY public.review ALTER COLUMN id_review SET DEFAULT nextval('public.review_id_review_seq'::regclass);
 ?   ALTER TABLE public.review ALTER COLUMN id_review DROP DEFAULT;
       public          postgres    false    221    220    221            �           2604    119412    type_member id_type    DEFAULT     z   ALTER TABLE ONLY public.type_member ALTER COLUMN id_type SET DEFAULT nextval('public.type_member_id_type_seq'::regclass);
 B   ALTER TABLE public.type_member ALTER COLUMN id_type DROP DEFAULT;
       public          postgres    false    226    227    227            �           2604    119371    user id_user    DEFAULT     n   ALTER TABLE ONLY public."user" ALTER COLUMN id_user SET DEFAULT nextval('public.user_id_user_seq'::regclass);
 =   ALTER TABLE public."user" ALTER COLUMN id_user DROP DEFAULT;
       public          postgres    false    217    216    217            5          0    119359    content 
   TABLE DATA           H   COPY public.content (id_content, name, description, awards) FROM stdin;
    public          postgres    false    215   (E       =          0    119393    journal 
   TABLE DATA           X   COPY public.journal (id_writing, id_user, id_content, id_rating, id_review) FROM stdin;
    public          postgres    false    223   rE       ?          0    119400    member 
   TABLE DATA           G   COPY public.member (id_member, name_member, awards_member) FROM stdin;
    public          postgres    false    225   �E       C          0    119418    part_member 
   TABLE DATA           N   COPY public.part_member (id_part, id_member, id_type, id_content) FROM stdin;
    public          postgres    false    229   �E       9          0    119377    rating 
   TABLE DATA           H   COPY public.rating (id_rating, id_user, id_content, rating) FROM stdin;
    public          postgres    false    219   $F       ;          0    119384    review 
   TABLE DATA           \   COPY public.review (id_review, id_content, id_user, text_review, recomendation) FROM stdin;
    public          postgres    false    221   LF       A          0    119409    type_member 
   TABLE DATA           ;   COPY public.type_member (id_type, type_member) FROM stdin;
    public          postgres    false    227   �F       7          0    119368    user 
   TABLE DATA           I   COPY public."user" (id_user, login, password, email, gender) FROM stdin;
    public          postgres    false    217   �F       R           0    0    content_id_content_seq    SEQUENCE SET     E   SELECT pg_catalog.setval('public.content_id_content_seq', 13, true);
          public          postgres    false    214            S           0    0    journal_id_writing_seq    SEQUENCE SET     D   SELECT pg_catalog.setval('public.journal_id_writing_seq', 4, true);
          public          postgres    false    222            T           0    0    member_id_member_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.member_id_member_seq', 11, true);
          public          postgres    false    224            U           0    0    part_member_id_part_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.part_member_id_part_seq', 44, true);
          public          postgres    false    228            V           0    0    rating_id_rating_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.rating_id_rating_seq', 10, true);
          public          postgres    false    218            W           0    0    review_id_review_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('public.review_id_review_seq', 2, true);
          public          postgres    false    220            X           0    0    type_member_id_type_seq    SEQUENCE SET     E   SELECT pg_catalog.setval('public.type_member_id_type_seq', 5, true);
          public          postgres    false    226            Y           0    0    user_id_user_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.user_id_user_seq', 35, true);
          public          postgres    false    216            �           2606    119366    content content_pkey 
   CONSTRAINT     Z   ALTER TABLE ONLY public.content
    ADD CONSTRAINT content_pkey PRIMARY KEY (id_content);
 >   ALTER TABLE ONLY public.content DROP CONSTRAINT content_pkey;
       public            postgres    false    215            �           2606    119398    journal journal_pkey 
   CONSTRAINT     Z   ALTER TABLE ONLY public.journal
    ADD CONSTRAINT journal_pkey PRIMARY KEY (id_writing);
 >   ALTER TABLE ONLY public.journal DROP CONSTRAINT journal_pkey;
       public            postgres    false    223            �           2606    119407    member member_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public.member
    ADD CONSTRAINT member_pkey PRIMARY KEY (id_member);
 <   ALTER TABLE ONLY public.member DROP CONSTRAINT member_pkey;
       public            postgres    false    225            �           2606    119423    part_member part_member_pkey 
   CONSTRAINT     _   ALTER TABLE ONLY public.part_member
    ADD CONSTRAINT part_member_pkey PRIMARY KEY (id_part);
 F   ALTER TABLE ONLY public.part_member DROP CONSTRAINT part_member_pkey;
       public            postgres    false    229            �           2606    119382    rating rating_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public.rating
    ADD CONSTRAINT rating_pkey PRIMARY KEY (id_rating);
 <   ALTER TABLE ONLY public.rating DROP CONSTRAINT rating_pkey;
       public            postgres    false    219            �           2606    119389    review review_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public.review
    ADD CONSTRAINT review_pkey PRIMARY KEY (id_review);
 <   ALTER TABLE ONLY public.review DROP CONSTRAINT review_pkey;
       public            postgres    false    221            �           2606    119416    type_member type_member_pkey 
   CONSTRAINT     _   ALTER TABLE ONLY public.type_member
    ADD CONSTRAINT type_member_pkey PRIMARY KEY (id_type);
 F   ALTER TABLE ONLY public.type_member DROP CONSTRAINT type_member_pkey;
       public            postgres    false    227            �           2606    119435    user uniq_email 
   CONSTRAINT     M   ALTER TABLE ONLY public."user"
    ADD CONSTRAINT uniq_email UNIQUE (email);
 ;   ALTER TABLE ONLY public."user" DROP CONSTRAINT uniq_email;
       public            postgres    false    217            �           2606    119433    user uniq_log 
   CONSTRAINT     K   ALTER TABLE ONLY public."user"
    ADD CONSTRAINT uniq_log UNIQUE (login);
 9   ALTER TABLE ONLY public."user" DROP CONSTRAINT uniq_log;
       public            postgres    false    217            �           2606    119375    user user_pkey 
   CONSTRAINT     S   ALTER TABLE ONLY public."user"
    ADD CONSTRAINT user_pkey PRIMARY KEY (id_user);
 :   ALTER TABLE ONLY public."user" DROP CONSTRAINT user_pkey;
       public            postgres    false    217            �           2606    119462 H   part_member Такая роль в этом фильме уже есть 
   CONSTRAINT     �   ALTER TABLE ONLY public.part_member
    ADD CONSTRAINT "Такая роль в этом фильме уже есть" UNIQUE (id_member, id_type, id_content) INCLUDE (id_member, id_type, id_content);
 t   ALTER TABLE ONLY public.part_member DROP CONSTRAINT "Такая роль в этом фильме уже есть";
       public            postgres    false    229    229    229            5   :   x�34�,I-.q��+I�+Af��'e�d��qr#KaQYTi�i����� w�"      =   #   x�3�44�44���4�2s�88��b���� Jye      ?   4   x�34�0���6\�}aׅ}6q�rr^��pa#��qqq ��      C   +   x�31�44�4�44�21�44�4 3���F\&&PQ#�=... ���      9      x�34�44�44�4����� *      ;   &   x�3�44�44�L,NI,���2�44���Db���� ���      A   S   x����0��a ��$)(�X�g
�h+�6�"w�[a's�Xt�	�
��ɮz�ɿ��S}���W��D�olT�R0�      7   %   x�34�C��"��"N.#CN#�@9g�W� p�-     